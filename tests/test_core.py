import ctypes
import pytest
from tuix.core import engine, builders, scenes, registry, objects, buffers
from tuix.core import _structs

SCENE = b"TestScene"


@pytest.fixture(scope="module", autouse=True)
def tuix_session():
    assert engine.init() == 0, "tuix_init() failed"
    builders.register_standard()
    scenes.init_scene(SCENE)
    # scene must be selected before main_loop renders anything
    registry.registry.current_scene_name = SCENE
    yield
    engine.shutdown()


def _make_obj(builder, w=0.8, h=0.15, mt=0.1, ml=0.1):
    uid = objects.create_object(builder, SCENE, w, h, mt, ml)
    assert uid > 0
    ptr = buffers.get_buffer_by_uid(uid)
    assert ptr is not None and ptr.contents.obj
    return uid, ptr.contents.obj.contents


# ── Engine ──────────────────────────────────────────────────────────────────

class TestEngine:
    def test_main_loop_does_not_crash(self):
        engine.main_loop()


# ── Scene management ─────────────────────────────────────────────────────────

class TestScenes:
    def test_init_duplicate_is_noop(self):
        result = scenes.init_scene(SCENE)
        assert result in (0, 1)

    def test_init_and_free_new_scene(self):
        scenes.init_scene(b"Tmp")
        scenes.free_scene(b"Tmp")

    def test_clear_scene(self):
        scenes.clear_scene(SCENE)


# ── Registry ─────────────────────────────────────────────────────────────────

class TestRegistry:
    def test_registry_layout_matches_c(self):
        expected = (
            ctypes.sizeof(_structs.TuixScenes)
            + ctypes.sizeof(_structs.TuixSubcycles)
            + ctypes.sizeof(_structs.TuixBuilders)
        )
        assert _structs.TuixRegistry.current_scene_name.offset == expected

    def test_set_scene_name_bytes(self):
        registry.registry.current_scene_name = SCENE
        assert registry.registry.current_scene_name == SCENE

    def test_set_scene_name_str_auto_encodes(self):
        registry.registry.current_scene_name = "TestScene"
        registry.registry.current_scene_name = SCENE


# ── Buffers / object creation ────────────────────────────────────────────────

class TestBuffers:
    def test_unknown_uid_returns_none(self):
        assert buffers.get_buffer_by_uid(999999) is None

    def test_create_returns_valid_uid(self):
        uid, obj = _make_obj(builders.PROGRESSBAR)
        assert obj.uid == uid
        buffers.free_buffer(SCENE, uid)

    def test_all_builders_create_objects(self):
        uids = []
        for i, bld in enumerate((builders.PROGRESSBAR, builders.CHOICE, builders.INPUT, builders.CANVAS)):
            uid, obj = _make_obj(bld, 0.5, 0.1, 0.05 + i * 0.18, 0.1)
            assert obj.uid == uid
            uids.append(uid)
        for uid in uids:
            buffers.free_buffer(SCENE, uid)


# ── Progressbar ──────────────────────────────────────────────────────────────

class TestProgressbar:
    @pytest.fixture(autouse=True)
    def pb(self):
        self.uid, self.obj = _make_obj(builders.PROGRESSBAR)
        yield
        buffers.free_buffer(SCENE, self.uid)

    def test_set_value_zero(self):
        assert objects.tuix_progressbar_set_value(self.obj, 0.0) == 0

    def test_set_value_full(self):
        assert objects.tuix_progressbar_set_value(self.obj, 1.0) == 0

    def test_set_value_mid(self):
        assert objects.tuix_progressbar_set_value(self.obj, 0.5) == 0

    def test_set_style(self):
        assert objects.tuix_progressbar_set_style(
            self.obj, ord('#'), ord('-'), 80, 200, 120, 60, 60, 60
        ) == 0


# ── Choice ───────────────────────────────────────────────────────────────────

class TestChoice:
    OPTIONS = [b"Alpha", b"Beta", b"Gamma"]

    @pytest.fixture(autouse=True)
    def ch(self):
        self.uid, self.obj = _make_obj(builders.CHOICE)
        objects.tuix_choice_set_options(self.obj, self.OPTIONS)
        yield
        buffers.free_buffer(SCENE, self.uid)

    def test_not_confirmed_initially(self):
        assert not objects.tuix_choice_is_confirmed(self.obj)

    def test_initial_selection_in_range(self):
        idx = objects.tuix_choice_get_selected(self.obj)
        assert 0 <= idx < len(self.OPTIONS)

    def test_reset_clears_confirmation(self):
        objects.tuix_choice_reset(self.obj)
        assert not objects.tuix_choice_is_confirmed(self.obj)


# ── Input ────────────────────────────────────────────────────────────────────

class TestInput:
    @pytest.fixture(autouse=True)
    def inp(self):
        self.uid, self.obj = _make_obj(builders.INPUT)
        yield
        buffers.free_buffer(SCENE, self.uid)

    def test_not_submitted_initially(self):
        assert not objects.tuix_input_is_submitted(self.obj)

    def test_set_placeholder_does_not_crash(self):
        objects.tuix_input_set_placeholder(self.obj, b"Type here...")

    def test_get_text_initially_empty(self):
        text = objects.tuix_input_get_text(self.obj)
        assert text in (b"", b"\x00", None, "")

    def test_reset_clears_submission(self):
        objects.tuix_input_reset(self.obj)
        assert not objects.tuix_input_is_submitted(self.obj)


# ── Canvas ───────────────────────────────────────────────────────────────────

class TestCanvas:
    @pytest.fixture(autouse=True)
    def cv(self):
        self.uid, self.obj = _make_obj(builders.CANVAS, 0.9, 0.7, 0.1, 0.05)
        # width/height are filled in by the geometry resolver on the first pass
        engine.main_loop()
        ptr = buffers.get_buffer_by_uid(self.uid)
        self.w = ptr.contents.width
        self.h = ptr.contents.height
        yield
        buffers.free_buffer(SCENE, self.uid)

    def test_geometry_resolved_after_main_loop(self):
        assert self.w > 0 and self.h > 0

    def test_set_pixel(self):
        assert objects.tuix_canvas_set_pixel(self.obj, 0, 0, b'*', 255, 0, 128, 0, 0, 0) == 0

    def test_draw_rect_outline(self):
        assert objects.tuix_canvas_draw_rect(
            self.obj, 0, 0, self.w, self.h, b'.', 0, 80, 80, 80, 0, 0, 0
        ) == 0

    def test_draw_rect_filled(self):
        assert objects.tuix_canvas_draw_rect(
            self.obj, 1, 1, self.w - 2, self.h - 2, b'#', 1, 200, 100, 50, 0, 0, 0
        ) == 0

    def test_draw_circle(self):
        r = min(self.w, self.h) // 4
        assert objects.tuix_canvas_draw_circle(
            self.obj, self.w // 2, self.h // 2, r, b'O', 0, 255, 255, 0, 0, 0, 0
        ) == 0

    def test_draw_line(self):
        assert objects.tuix_canvas_draw_line(
            self.obj, 0, 0, self.w - 1, self.h - 1, b'/', 200, 200, 200, 0, 0, 0
        ) == 0

    def test_draw_text(self):
        assert objects.tuix_canvas_draw_text(
            self.obj, 1, 1, b"hello", 255, 255, 255, 0, 0, 0
        ) == 0

    def test_out_of_bounds_returns_error(self):
        # coordinates beyond the canvas should be rejected, not crash
        r = objects.tuix_canvas_set_pixel(
            self.obj, self.w + 100, self.h + 100, b'X', 255, 255, 255, 0, 0, 0
        )
        assert r != 0

    def test_sprite_cache_roundtrip(self):
        sprite = (_structs.TuixPixel * 1)()
        sprite[0].sym = b'@' + (b'\x00' * 7)
        sprite[0].styles.fg = _structs.TuixRGBTuple(255, 128, 0)
        sprite[0].styles.custom_fg = 1
        sid = objects.tuix_canvas_cache_sprite(self.obj, 1, 1, ctypes.addressof(sprite))
        assert sid >= 0
        assert objects.tuix_canvas_draw_cached_sprite(self.obj, sid, 2, 2) == 0
        objects.tuix_canvas_free_cached_sprite(self.obj, sid)
