from setuptools import setup, find_namespace_packages


with open("README.md", "r", encoding="utf-8") as fp:
    long_description = fp.read()


setup(
    name="iCE40-I2C-LiteX-integration",
    version="2024.10.2",
    description="LiteX module of the Lattice iCE40 UltraPlus I2C Hard IP",
    long_description=long_description,
    long_description_content_type="text/markdown",
    author="Signaloid",
    author_email="developer-support@signaloid.com",
    url="https://signaloid.com",
    download_url="https://github.com/signaloid/iCE40-I2C-LiteX-integration",
    license="BSD 2-clause",
    python_requires="~=3.7",
    packages=find_namespace_packages(where="src"),
    package_dir={"": "src"},
    install_requires=[
        "litex",
    ],
    package_data={
        'iCE40_I2C_LiteX_integration.c_driver_library': ['*.c', '*.h'],
    },
    keywords="HDL ASIC FPGA hardware design LiteX I2C Lattice iCE40",
    classifiers=[
        "Topic :: Scientific/Engineering :: Electronic Design Automation (EDA)",
        "Environment :: Console",
        "Development Status :: 3 - Alpha",
        "Intended Audience :: Developers",
        "License :: OSI Approved :: BSD License",
        "Operating System :: OS Independent",
        "Programming Language :: Python",
    ],
)
