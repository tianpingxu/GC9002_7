#Used for GC9002AHMI, add CRC at the end of the cpu.bin file
#by Yu Ning

PROJECT_NAME=GC9002AHMI
BIN_FILE_NAME='cpu.bin'
OUTPUT_BIN_FILE_NAME='cpu1.bin'
SREC_PATH='C:\SREC\'

OBJCOPY_NAME=riscv64-unknown-elf-objcopy
OBJCOPY_EXE=$OBJCOPY_NAME.exe
OBJCOPY_PATH=CDKRepo/Toolchain/XTGccElfNewlib/V2.6.1/R/bin

#echo $CDKPath
cd $CDKPath/../$OBJCOPY_PATH
OBJCOPY_FULL_PATH=$(pwd)

cd $ProjectPath
cd Obj

echo "-------------------------------------"
echo "Create .bin from .elf firstly"
echo "-------------------------------------"
echo $OBJCOPY_FULL_PATH/$OBJCOPY_EXE -O binary $PROJECT_NAME.elf $BIN_FILE_NAME
$OBJCOPY_FULL_PATH/$OBJCOPY_EXE -O binary $PROJECT_NAME.elf $BIN_FILE_NAME

if [ -f $BIN_FILE_NAME ];then
echo "Add CRC32 at the end of the cpu.bin"
echo "-------------------------------------"

echo 
myfilesize=$(ls -l $BIN_FILE_NAME | awk '{ print $5 }')
echo $myfilesize

echo $SREC_PATH\srec_cat.exe $BIN_FILE_NAME -Binary -crop 0 $myfilesize  -crc32-b-e $myfilesize  -o $OUTPUT_BIN_FILE_NAME -Binary
$SREC_PATH\srec_cat.exe $BIN_FILE_NAME -Binary -crop 0 $myfilesize  -crc32-l-e $myfilesize  -o $OUTPUT_BIN_FILE_NAME -Binary

rm -f $BIN_FILE_NAME
else
echo
fi
