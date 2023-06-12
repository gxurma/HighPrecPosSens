import barcode
from barcode.writer import SVGWriter
  
#Define content of the barcode as a string
#number = 'A'+ '5'*50 + 'A'
#number = 'W'*50
number ="A1B1C1D1E1F1G1H1I1J1K1L1M1N1O1P1Q1R1S1T1U1V1W1X1Y1Z1A2B2C2D2E2F2G2H2I2J2K2L2M2N2O2P2Q2R2S2T2U2V2W2X2Y2Z2"

print(number)
#for i in barcode.PROVIDED_BARCODES:

#Get the required barcode format
# barcode_format = barcode.get_barcode_class("codabar")
barcode_format = barcode.get_barcode_class("code39")


#Generate barcode and render as image
my_barcode = barcode_format(number, writer=SVGWriter()) #,module_width=2.0, module_height=100.0)

options = dict(module_width=1.0, module_height=200.0)

print(options)

#SVGWriter.set_options(False, options)


print(my_barcode)

#Save barcode as svg
my_barcode.save("generated_barcode codabar", options)
