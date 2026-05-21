from pathlib import Path
import os
import sys

from setuptools import Extension, find_namespace_packages, setup

try:
    from Cython.Build import cythonize
except ImportError:
    cythonize = None


ROOT = Path(__file__).resolve().parent
CORE_DIR = ROOT / "src" / "tuix" / "core"


def rel(*parts: str) -> str:
    return str(ROOT.joinpath(*parts))


if sys.platform == "win32":
    # Large generated Cython translation units are much more reliable on MSVC
    # with /bigobj, and /GL tends to make failures harder to diagnose.
    extra_compile = ["/bigobj", "/GL-"]
    extra_link = []
elif sys.platform == "darwin":
    extra_compile = []
    extra_link = []
else:
    # Use the compiler+linker form for thread support rather than only -lpthread.
    extra_compile = ["-pthread"]
    extra_link = ["-pthread"]


sources = [
    "src/tuix/core/main.c",
    "src/tuix/core/event_shim.c",
    "src/tuix/core/init.c",
    "src/tuix/core/rendering.c",
    "src/tuix/core/cache_manager_stub.c",
    "src/tuix/core/buffers.c",
    "src/tuix/core/buffer_manager.c",
    "src/tuix/core/object_manager.c",
    "src/tuix/core/compositor/compositor.c",
    "src/tuix/core/compositor/geometry_resolver.c",
    "src/tuix/core/compositor/merge_utils.c",
    "src/tuix/core/content_builder/builder_utils.c",
    "src/tuix/core/content_builder/builders_registrant.c",
    "src/tuix/core/content_builder/builders_router.c",
    "src/tuix/core/content_builder/builders/canvas_builder.c",
    "src/tuix/core/content_builder/builders/choice_builder.c",
    "src/tuix/core/content_builder/builders/input_builder.c",
    "src/tuix/core/content_builder/builders/progressbar_builder.c",
    "src/tuix/core/content_builder/builders/text_builder.c",
    "src/tuix/core/content_builder/builders/box_builder.c",
    "src/tuix/core/content_builder/builders/divider_builder.c",
    "src/tuix/core/content_builder/builders/badge_builder.c",
    "src/tuix/core/content_builder/builders/button_builder.c",
    "src/tuix/core/content_builder/builders/tag_builder.c",
    "src/tuix/core/content_builder/builders/status_builder.c",
    "src/tuix/core/content_builder/builders/menu_builder.c",
    "src/tuix/core/content_builder/builders/scroll_container_builder.c",
    "src/tuix/core/content_builder/builders/stack_builder.c",
    "src/tuix/core/content_builder/builders/splitpane_builder.c",
    "src/tuix/core/content_builder/builders/grid_builder.c",
    "src/tuix/core/content_builder/builders/checkbox_builder.c",
    "src/tuix/core/content_builder/builders/listview_builder.c",
    "src/tuix/core/content_builder/builders/textarea_builder.c",
    "src/tuix/core/content_builder/builders/dialog_builder.c",
    "src/tuix/core/input/input.c",
    "src/tuix/core/subcycles/subcycle_registrant.c",
    "src/tuix/core/batch_executor.c",
]


missing_sources = [path for path in sources if not (ROOT / path).exists()]
if missing_sources:
    missing = "\n".join(missing_sources)
    raise FileNotFoundError(f"setup.py source list contains missing files:\n{missing}")


generated_c = CORE_DIR / "_tuix_cy.c"
pyx_source = CORE_DIR / "_tuix_cy.pyx"
use_cython = os.environ.get("TUIX_USE_CYTHON") == "1"

if use_cython:
    if cythonize is None:
        raise RuntimeError("TUIX_USE_CYTHON=1 was set, but Cython is not installed.")
    bridge_sources = [str(pyx_source.relative_to(ROOT))]
else:
    bridge = generated_c if generated_c.exists() else pyx_source
    bridge_sources = [str(bridge.relative_to(ROOT))]

if bridge_sources[0].endswith(".pyx") and cythonize is None:
    raise RuntimeError(
        "Cython is required because src/tuix/core/_tuix_cy.c is missing. "
        "Install Cython or regenerate the checked-in C file."
    )


include_dirs = [
    "src/tuix/core",
    "src/tuix/core/compositor",
    "src/tuix/core/content_builder",
    "src/tuix/core/content_builder/builders",
    "src/tuix/core/input",
    "src/tuix/core/subcycles",
]


extension = Extension(
    name="tuix.core._tuix_cy",
    sources=bridge_sources + sources,
    include_dirs=include_dirs,
    define_macros=[("TUIX_BUILDING_CORE", "1")],
    extra_compile_args=extra_compile,
    extra_link_args=extra_link,
)

ext_modules = [extension]
if bridge_sources[0].endswith(".pyx"):
    ext_modules = cythonize(
        ext_modules,
        compiler_directives={"language_level": 3},
    )


setup(
    packages=find_namespace_packages(where="src", include=["tuix*"]),
    package_dir={"": "src"},
    ext_modules=ext_modules,
)
