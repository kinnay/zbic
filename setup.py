import os
import setuptools

description = \
	"ZBIC compression algorithm of Nintendo Switch."

this_directory = os.path.dirname(__file__)
with open(os.path.join(this_directory, "README.md"), "r") as file:
	long_description = file.read()

extensions = [setuptools.Extension(
	name = "zbic",
	sources = ["src/module.c", "src/zstd.c"],
    include_dirs = ["src"],
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
