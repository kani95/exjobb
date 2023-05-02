@pushd %~dp0
@echo Compiling C#...
"csc.exe" /target:library /out:"C:\Users\PC100166937\Desktop\ops\Examples\CppApps\PingPong\OpsPingPong\Generated\CSharp\OpsPingPong.dll" /r:"C:\Users\PC100166937\Desktop\ops\Examples\CppApps\PingPong\OpsPingPong\..\..\..\CSharp\Projects\OpsLibrary\OpsLibrary\bin\Debug\OpsLibrary.dll"  /recurse:"C:\Users\PC100166937\Desktop\ops\Examples\CppApps\PingPong\OpsPingPong\Generated\CSharp\*.cs"
@echo Compiling C# done.
@popd
