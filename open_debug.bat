@echo off
set outputdir=%cd%/build/
set sourcedir=%cd%/src/
devenv Syrax.sln /projectconfig "debug|x64"