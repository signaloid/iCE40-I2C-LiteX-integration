# 	Copyright (c) 2024, Signaloid.
#
# 	Permission is hereby granted, free of charge, to any person obtaining a copy
# 	of this software and associated documentation files (the "Software"), to
# 	deal in the Software without restriction, including without limitation the
# 	rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# 	sell copies of the Software, and to permit persons to whom the Software is
# 	furnished to do so, subject to the following conditions:
#
# 	The above copyright notice and this permission notice shall be included in
# 	all copies or substantial portions of the Software.
#
# 	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# 	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# 	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# 	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# 	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# 	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# 	DEALINGS IN THE SOFTWARE.


from litex.soc.integration.doc import AutoDoc, ModuleDoc
from litex.soc.integration.soc import (
    Instance,
    Module,
    Signal,
)
from litex.soc.interconnect.csr import (
    AutoCSR,
    CSRAccess,
    CSRField,
    CSRStatus,
    CSRStorage,
)


class ICE40UP_I2C(Module, AutoCSR, AutoDoc):
    def __init__(
        self, scl_pin: Signal, sda_pin: Signal, sys_clk: Signal
    ) -> None:
        self.intro = ModuleDoc(
            """ICE40UP_I2C.
            Instantiates the ICE40UP5K's I2C hard IP interface.
            """
        )

        #   System Bus Control Signals
        self._sbctrl = CSRStorage(
            size=2,
            fields=[
                CSRField(
                    name="SBRWI",
                    description="""System Bus Read/Write input. R=0, W=1""",
                ),
                CSRField(
                    name="SBSTBI", description="""System Bus Strobe Signal"""
                ),
            ],
        )

        #   System Bus Status Signals
        self._sbstatus = CSRStatus(
            size=1,
            fields=[
                CSRField(
                    name="SBACKO",
                    description="""System Bus Acknowledgement""",
                    access=CSRAccess.ReadOnly,
                ),
            ],
        )

        #   System Bus Control Registers Address
        self._sbadri = CSRStorage(
            size=4, description="System Bus Control registers address."
        )

        #   System Bus Data Input
        self._sbdati = CSRStorage(size=8, description="System Data Input.")

        #   System Bus Data Output
        self._sbdato = CSRStatus(size=8, description="System Data Output.")

        #   I2C Signals
        sdai = Signal()
        sdao = Signal()
        scli = Signal()
        sclo = Signal()
        sdaoe = Signal()
        scloe = Signal()

        #   I2C Hard IP
        self.specials += Instance(
            "SB_I2C",
            #   I2C Slave Parameters
            #   Use Upper Left Corner I2C Hard IP
            p_I2C_SLAVE_INIT_ADDR="0b1111100001",
            p_BUS_ADDR74="0b0001",
            #   System Bus Signals
            i_SBCLKI=sys_clk,
            i_SBRWI=self._sbctrl.fields.SBRWI,
            i_SBSTBI=self._sbctrl.fields.SBSTBI,
            o_SBACKO=self._sbstatus.fields.SBACKO,
            #   System Bus Control Registers Address
            #   Hardwire top address bits to always use the upper left corner
            #   I2C Hard IP
            i_SBADRI7=0b0,
            i_SBADRI6=0b0,
            i_SBADRI5=0b0,
            i_SBADRI4=0b1,
            i_SBADRI3=self._sbadri.storage[3],
            i_SBADRI2=self._sbadri.storage[2],
            i_SBADRI1=self._sbadri.storage[1],
            i_SBADRI0=self._sbadri.storage[0],
            #   System Bus Data Input
            i_SBDATI7=self._sbdati.storage[7],
            i_SBDATI6=self._sbdati.storage[6],
            i_SBDATI5=self._sbdati.storage[5],
            i_SBDATI4=self._sbdati.storage[4],
            i_SBDATI3=self._sbdati.storage[3],
            i_SBDATI2=self._sbdati.storage[2],
            i_SBDATI1=self._sbdati.storage[1],
            i_SBDATI0=self._sbdati.storage[0],
            #   System Bus Data Output
            o_SBDATO7=self._sbdato.status[7],
            o_SBDATO6=self._sbdato.status[6],
            o_SBDATO5=self._sbdato.status[5],
            o_SBDATO4=self._sbdato.status[4],
            o_SBDATO3=self._sbdato.status[3],
            o_SBDATO2=self._sbdato.status[2],
            o_SBDATO1=self._sbdato.status[1],
            o_SBDATO0=self._sbdato.status[0],
            #   I2C Signals
            i_SCLI=scli,
            o_SCLO=sclo,
            o_SCLOE=scloe,
            i_SDAI=sdai,
            o_SDAO=sdao,
            o_SDAOE=sdaoe,
        )

        #   Multiplexers for using the same SDA/SCL pins for input/output
        self.specials += Instance(
            "SB_IO",
            #   Parameters
            #   Pin input with pin output tristate
            p_PIN_TYPE=0b101001,
            #   Enable pullup resistors
            p_PULLUP=0b1,
            #   I2C Signals
            io_PACKAGE_PIN=scl_pin,
            i_OUTPUT_ENABLE=scloe,
            i_D_OUT_0=sclo,
            o_D_IN_0=scli,
        )

        self.specials += Instance(
            "SB_IO",
            #   Parameters
            #   Pin input with pin output tristate
            p_PIN_TYPE=0b101001,
            #   Enable pullup resistors
            p_PULLUP=0b1,
            #   I2C Signals
            io_PACKAGE_PIN=sda_pin,
            i_OUTPUT_ENABLE=sdaoe,
            i_D_OUT_0=sdao,
            o_D_IN_0=sdai,
        )
