
import os
import setuptools

description = \
    "ZBIC compression algorithm of Nintendo Switch."

if os.environ.get("GITHUB_WORKSPACE"):
    # This is a workaround for the GitHub actions build on Windows.
    # Relative paths aren't working.
    this_directory = os.environ["GITHUB_WORKSPACE"]
else:
    this_directory = os.path.dirname(__file__)

with open(os.path.join(this_directory, "README.md"), "r") as file:
    long_description = file.read()

extensions = [setuptools.Extension(
    name = "zbic",
    sources = [os.path.join(this_directory, "src", "module.c"), os.path.join(this_directory, "src", "zstd.c")],
    include_dirs = [os.path.join(this_directory, "src")],
    define_macros = [("ZSTD_ZBIC_SUPPORT", "1")]
)]

setuptools.setup(
    name = "zbic",
    version = "1.0.0.post1",
    description = description,
    long_description = long_description,
    long_description_content_type = "text/markdown",
    author = "Yannik Marchand",
    author_email = "ymarchand@me.com",
    url = "https://github.com/kinnay/zbic",
    license = "GPLv2",
    ext_modules = extensions
)
