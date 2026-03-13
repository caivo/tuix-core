from setuptools import setup, Extension
import sys
import os
try:
    from Cython.Build import cythonize
    has_cython = True
except Exception:
    cythonize = None
    has_cython = False

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
    "src/tuix/core/input/input.c",
    "src/tuix/core/subcycles/subcycle_registrant.c",
    "src/tuix/core/subcycles/subcycles_manager.c",
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

cython_sources = ["src/tuix/core/_tuix_cy.pyx"] if has_cython else ["src/tuix/core/_tuix_cy.c"]
tuix_cy = Extension(
    name="tuix.core._tuix_cy",
    sources=cython_sources + sources,
    include_dirs=include_dirs,
    extra_compile_args=extra_compile,
    extra_link_args=extra_link,
)

setup(
    ext_modules=cythonize([tuix_cy]) if has_cython else [tuix_cy],
)