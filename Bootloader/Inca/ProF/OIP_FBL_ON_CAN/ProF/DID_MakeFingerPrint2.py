from datetime import date
import os


def int_to_bcd(n):
    return int(str(n), base=16)

def Do_Did_FingerPrintData(bin_path):
    'Make the FingerPrint'
    isCompleted = False
    TesterSerialNumber= [0x00, 0x01, 0x02, 0x03, 0x04, 0x05]
    NumberArray = []
    WriteBuf = []

    print("Use the default value of TesterSerialNumber")

    today = date.today()

# dd/mm/yy
    d1 = today.strftime("%y/%m/%d")
    YYMMDD = list(map(int, d1.split('/')))
    YYMMDD[0] = int_to_bcd(YYMMDD[0])
    YYMMDD[1] = int_to_bcd(YYMMDD[1])
    YYMMDD[2] = int_to_bcd(YYMMDD[2])
    print("Year: ", hex(YYMMDD[0]))
    print("Month: ", hex(YYMMDD[1]))
    print("Day: ", hex(YYMMDD[2]))
    print("TesterSerialNumber:" , TesterSerialNumber)

    WriteBuf.extend(YYMMDD)
    WriteBuf.extend(list(TesterSerialNumber))
    newFile = open(os.path.join(bin_path,'Application_softwarefingerprint.bin'), "wb")
    newFileByteArray = bytearray(WriteBuf)
    newFile.write(newFileByteArray)
    newFile.close()

if __name__ == '__main__':
    print("---->Welcome to FingerPrint Tool<----")
    cwd = os.getcwd()
    Do_Did_FingerPrintData(cwd)