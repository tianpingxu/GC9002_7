#Used for bootloader upgrade itself, add information to the head of the bootloader.bin file
#by Zhu Jiahai

PROJECT_NAME=Bootloader
BIN_FILE_NAME='boot_tmp.bin'
OUTPUT_BIN_FILE_NAME='bootloader.bin'
MAGIC_NUM=0x48494948
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
echo "-------------------------------------"
echo "Add Head Infomation  at the beginning of .bin file"
echo "-------------------------------------"

myfilesize=$(ls -l $BIN_FILE_NAME | awk '{ print $5 }')
echo "filesize:" $myfilesize
myfilesizeAddCrcSize=$[myfilesize+4]
#echo $myfilesizeAddCrcSize
#echo $SREC_PATH\srec_cat.exe  $BIN_FILE_NAME  -Binary -crop 0 $myfilesize -crc32-l-e $myfilesize   -o boot.bin -Binary
$SREC_PATH\srec_cat.exe  $BIN_FILE_NAME  -Binary -crop 0 $myfilesize -crc32-l-e $myfilesize   -o boot.bin -Binary
#echo $SREC_PATH\srec_cat.exe  boot.bin  -Binary -crop $myfilesize $myfilesizeAddCrcSize -offset -$myfilesize -o bootCrc.bin -Binary
$SREC_PATH\srec_cat.exe  boot.bin  -Binary -crop $myfilesize $myfilesizeAddCrcSize -offset -$myfilesize -o bootCrc.bin -Binary
echo $SREC_PATH\srec_cat.exe  -generate 0x0 0x4 -constant-l-e $myfilesize 4 bootCrc.bin -Binary -offset 0x4 -generate 0x8 0xC -constant-l-e $MAGIC_NUM 4 -generate 0xC 0x10 -constant-l-e 0x0 4 $BIN_FILE_NAME -Binary -offset 0x10  -o $OUTPUT_BIN_FILE_NAME -Binary
$SREC_PATH\srec_cat.exe  -generate 0x0 0x4 -constant-l-e $myfilesize 4 bootCrc.bin -Binary -offset 0x4 -generate 0x8 0xC -constant-l-e $MAGIC_NUM 4 -generate 0xC 0x10 -constant-l-e 0x0 4 $BIN_FILE_NAME -Binary -offset 0x10  -o $OUTPUT_BIN_FILE_NAME -Binary
rm -f boot.bin
rm -f bootCrc.bin
rm -f $BIN_FILE_NAME
else
echo "failure as .bin file not created"
fi
#sleep 10
