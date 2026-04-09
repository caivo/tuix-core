from setuptools import setup, Extension, find_namespace_packages
import sys
import os
from Cython.Build import cythonize

# Platform-specific linker flags
if sys.platform == "win32":
    extra_link = []
    extra_compile = []
elif sys.platform == "darwin":
    extra_link = []
    extra_compile = []
else:
    extra_link = ["-lpthread"]
    extra_compile = []

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
    "src/tuix/core/content_builder/builders/icon_builder.c",
    "src/tuix/core/content_builder/builders/tag_builder.c",
    "src/tuix/core/content_builder/builders/status_builder.c",
    "src/tuix/core/content_builder/builders/menu_builder.c",
    "src/tuix/core/content_builder/builders/scroll_container_builder.c",
    "src/tuix/core/input/input.c",
    "src/tuix/core/subcycles/subcycle_registrant.c",
    "src/tuix/core/batch_executor.c",
]

# Exclude some sources
exclude_sources = {"cache_manager.c", "rendering_halfblocks.c"}
sources = [s for s in sources if os.path.basename(s) not in exclude_sources]

include_dirs = [
    "src/tuix/core",
    "src/tuix/core/compositor",
    "src/tuix/core/content_builder",
    "src/tuix/core/content_builder/builders",
    "src/tuix/core/input",
    "src/tuix/core/subcycles",
]

cython_sources = ["src/tuix/core/_tuix_cy.pyx"]
tuix_cy = Extension(
    name="tuix.core._tuix_cy",
    sources=cython_sources + sources,
    include_dirs=include_dirs,
    define_macros=[("TUIX_BUILDING_CORE", "1")],
    extra_compile_args=extra_compile,
    extra_link_args=extra_link,
)

setup(
    packages=find_namespace_packages(where="src", include=["tuix*"]),
    package_dir={"": "src"},
    ext_modules=cythonize([tuix_cy], force=True),
)