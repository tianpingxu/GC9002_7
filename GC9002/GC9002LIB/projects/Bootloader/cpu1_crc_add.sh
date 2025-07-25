
BIN_FILE_NAME='Bootloader_0x08000000.bin'

cd Obj

if [ -f $BIN_FILE_NAME ];then
echo "Add CRC32  At the end of the document"
echo "-------------------------------------"

SREC_PATH='C:\SREC\'
echo 
myfilesize=$(ls -l $BIN_FILE_NAME | awk '{ print $5 }')
echo $myfilesize

echo $SREC_PATH\srec_cat.exe $BIN_FILE_NAME -Binary -crop 0 $myfilesize  -crc32-b-e $myfilesize  -o boot.bin -Binary
$SREC_PATH\srec_cat.exe $BIN_FILE_NAME -Binary -crop 0 $myfilesize  -crc32-l-e $myfilesize  -o boot.bin -Binary
else
echo
fi
