
from pathlib import Path

import setuptools

description = \
	"ZBIC compression algorithm of Nintendo Switch."

this_directory = Path(__file__).parent
long_description = (this_directory / "README.md").read_text()

extensions = [setuptools.Extension(
	name = "zbic",
	sources = ["src/module.c", "src/zstd.c"],
    include_dirs = ["src"],
	define_macros = [("ZSTD_ZBIC_SUPPORT", "1")]
)]

setuptools.setup(
	name = "zbic",
	version = "1.0.0",
	description = description,
	long_description = long_description,
    long_description_content_type = "text/markdown",
	author = "Yannik Marchand",
	author_email = "ymarchand@me.com",
	url = "https://github.com/kinnay/zbic",
	license = "GPLv2",
	ext_modules = extensions
)
