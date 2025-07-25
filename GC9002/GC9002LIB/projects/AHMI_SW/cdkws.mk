.PHONY: clean All Project_Title Project_Build Project_PostBuild

All: Project_Title Project_Build Project_PostBuild

Project_Title:
	@echo "----------Building project:[ GC9002AHMI - BuildSet ]----------"

Project_Build:
	@make -r -f GC9002AHMI.mk -j 12 -C  ./ 

Project_PostBuild:
	@echo Executing Post Build commands ...
	@export CDKPath="D:/C-Sky/CDK" CDK_VERSION="V2.22.0" ProjectName="GC9002AHMI" ProjectPath="D:/GC9002_7/GC9002/GC9002LIB/projects/AHMI_SW/" && D:/GC9002_7/GC9002/GC9002LIB/projects/AHMI_SW/cpu1_crc_add.sh
	@echo Done


clean:
	@echo "----------Cleaning project:[ GC9002AHMI - BuildSet ]----------"

