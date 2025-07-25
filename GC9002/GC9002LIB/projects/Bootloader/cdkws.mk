.PHONY: clean All Project_Title Project_Build Project_PostBuild

All: Project_Title Project_Build Project_PostBuild

Project_Title:
	@echo "----------Building project:[ Bootloader - BuildSet ]----------"

Project_Build:
	@make -r -f Bootloader.mk -j 8 -C  ./ 

Project_PostBuild:
	@echo Executing Post Build commands ...
	@export CDKPath="E:/C-Sky/CDK" CDK_VERSION="V2.20.0" ProjectName="Bootloader" ProjectPath="E:/a_/GC9002_1024x600/GC9002/GC9002/GC9002LIB/projects/Bootloader/" && E:/a_/GC9002_1024x600/GC9002/GC9002/GC9002LIB/projects/Bootloader/boot_headInfo_add.sh 
	@echo Done


clean:
	@echo "----------Cleaning project:[ Bootloader - BuildSet ]----------"

