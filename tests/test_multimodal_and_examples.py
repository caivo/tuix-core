import ctypes
from pathlib import Path

import pytest

from tuix.core import _lib
from tuix.core import _structs
from tuix.core import builders
from tuix.core import buffers
from tuix.core import engine
from tuix.core import input
from tuix.core import objects
from tuix.core import registry
from tuix.core import scenes

SCENE = b"TestSceneMultimodal"


@pytest.fixture(scope="module", autouse=True)
def tuix_session():
    assert engine.init() == 0, "tuix_init() failed"
    builders.register_standard()
    scenes.init_scene(SCENE)
    registry.registry.current_scene_name = SCENE
    yield
    engine.shutdown()


def _make_obj(builder, w=0.5, h=0.2, mt=0.1, ml=0.1):
    uid = objects.create_object(builder, SCENE, w, h, mt, ml)
    assert uid > 0
    obj = objects.get_object_by_uid(uid)
    assert obj is not None
    return uid, obj


class TestMultimodalApis:
    def test_snapshot_has_keyboard_and_mouse_channels(self):
        snap = input.get_snapshot()
        assert hasattr(snap, "keyboard")
        assert hasattr(snap, "mouse")
        assert hasattr(snap, "consumed_keyboard")
        assert hasattr(snap, "consumed_mouse")
        assert snap.consumed_keyboard in (False, True)
        assert snap.consumed_mouse in (False, True)

    def test_scene_focus_routing_primitives(self):
        uid_choice, _ = _make_obj(builders.CHOICE, 0.4, 0.3, 0.2, 0.05)
        uid_input, _ = _make_obj(builders.INPUT, 0.4, 0.1, 0.2, 0.55)

        assert scenes.set_focus(SCENE, uid_choice) == 0
        assert scenes.set_previous_focus(SCENE) == 0
        assert scenes.set_focus(SCENE, -123456) == -1

        buffers.free_buffer(SCENE, uid_choice)
        buffers.free_buffer(SCENE, uid_input)

    def test_scene_getters_are_exposed(self):
        scene_addr = scenes.get_scene(SCENE)
        assert scene_addr != 0

        names = scenes.get_scenes()
        assert isinstance(names, list)
        assert any(n == SCENE or n == SCENE.decode("utf-8") for n in names)

    def test_low_level_object_and_buffer_init_api(self):
        obj_addr = _lib._mod.tuix_objects_new_object(
            builders.PROGRESSBAR, SCENE, 0.6, 0.1, 0.1, 0.1
        )
        assert obj_addr != 0

        obj_ptr = ctypes.cast(ctypes.c_void_p(obj_addr), ctypes.POINTER(_structs.TuixObject))
        uid = obj_ptr.contents.uid
        assert uid > 0

        _lib._mod.tuix_init_buffer(SCENE, obj_addr)
        snap = buffers.get_buffer_snapshot_by_uid(uid)
        assert isinstance(snap, dict)
        assert snap.get("uid") == -1
        assert snap.get("required_redraw") in (0, 1)

        buffers.free_buffer(SCENE, uid)
        _lib._mod.tuix_free_object_ptr(obj_addr)

    def test_rgb16_lock_and_cache_api(self):
        rgb = _lib._mod.tuix_rgb16(300, -5, 127)
        assert isinstance(rgb, tuple)
        assert len(rgb) == 3
        assert all(0 <= c <= 255 for c in rgb)

        _lib._mod.tuix_lock()
        _lib._mod.tuix_unlock()

        _lib._mod.tuix_cache_scenes()
        _lib._mod.tuix_restore_scenes()


class TestExamplesLayout:
    ROOT = Path(__file__).resolve().parents[1]
    EXAMPLES = ROOT / "examples"

    def test_examples_are_grouped_by_domain(self):
        assert (self.EXAMPLES / "widgets").is_dir()
        assert (self.EXAMPLES / "multimodal").is_dir()
        assert (self.EXAMPLES / "showcase").is_dir()

    def test_expected_examples_exist(self):
        expected = [
            self.EXAMPLES / "widgets" / "progressbar_dual.py",
            self.EXAMPLES / "widgets" / "choice_palette.py",
            self.EXAMPLES / "widgets" / "canvas_bounce.py",
            self.EXAMPLES / "multimodal" / "focus_routing.py",
            self.EXAMPLES / "showcase" / "sequential_journey.py",
        ]
        for p in expected:
            assert p.exists(), f"missing example: {p}"

    def test_legacy_flat_examples_removed(self):
        legacy = [
            self.EXAMPLES / "ex_progressbar.py",
            self.EXAMPLES / "ex_choice.py",
            self.EXAMPLES / "ex_canvas.py",
            self.EXAMPLES / "ex_multi_widget.py",
            self.EXAMPLES / "builders.py",
        ]
        for p in legacy:
            assert not p.exists(), f"legacy example should be removed: {p}"

    def test_examples_parse_as_python(self):
        paths = [
            self.EXAMPLES / "widgets" / "progressbar_dual.py",
            self.EXAMPLES / "widgets" / "choice_palette.py",
            self.EXAMPLES / "widgets" / "canvas_bounce.py",
            self.EXAMPLES / "multimodal" / "focus_routing.py",
            self.EXAMPLES / "showcase" / "sequential_journey.py",
        ]
        for p in paths:
            code = p.read_text(encoding="utf-8")
            compile(code, str(p), "exec")
