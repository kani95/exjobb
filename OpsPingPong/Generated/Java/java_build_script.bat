@pushd %~dp0
@echo Compiling Java...
@javac -version
@if not exist ".obj" (
@mkdir .obj
)
javac -cp "C:/Users/PC100166937/Desktop/ops/Tools/opsc/dist/OPSJLib.jar";"C:/Users/PC100166937/Desktop/ops/Tools/opsc/dist/ConfigurationLib.jar"; -d .obj @"debugger_buildinfo.ops_tmp"
jar cfm "OpsPingPong.jar" "manifest_adds.ops_tmp" -C .obj . 
@echo Compiling Java done.
@popd
