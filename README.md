# iCE40-I2C-LiteX-integration
This Python package provides integration of the iCE40 UltraPlus I2C hard IP interface with LiteX. It allows easy instantiation and control of the I2C hardware in FPGA designs using LiteX.

You can find an example design and application using this package on the  [Signaloid-C0-microSD-litex-I2C-demo](https://github.com/signaloid/Signaloid-C0-microSD-litex-I2C-demo) repository.

## Features
- Instantiates the iCE40 UltraPlus I2C hard IP interface
- Provides CSR (Control/Status Register) interfaces for I2C control
- Includes C driver library for software control of the I2C interface

**Note:** This package is exposing/instantiating only one of the two available I2C hard IP interfaces on the iCE40 UltraPlus FPGA. The other interface is not currently being utilized.

## Installation
You can install this package using pip:
```sh
pip install git+https://github.com/signaloid/iCE40-I2C-LiteX-integration.git
```

## Usage
To use this package in your LiteX design:
1. Import the ICE40UP_I2C class:

	```python
	from iCE40_I2C_LiteX_integration import ICE40UP_I2C
	```

2. Instantiate the I2C interface in your SoC:
	```python
	self.submodules.i2c = ICE40UP_I2C(scl_pin, sda_pin, self.crg.cd_sys.clk)
	```

3. Use the provided C driver library to control the I2C interface from software. For more details, refer to the header files in the `c_driver_library` directory.

## Requirements
- LiteX
- Python 3.7 or later
