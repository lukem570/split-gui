add:
	cmake -G Ninja -B build -DSPLITGUI_DEV=SPLITGUI_DEV
	ninja -C build

cube:
	python tools/testTools/test.py build/tests/cubeTest

scene:
	python tools/testTools/test.py build/tests/sceneTest

multi:
	python tools/testTools/test.py build/tests/multiSceneTest

text:
	python tools/testTools/test.py build/tests/textTest

textxml:
	python tools/testTools/test.py build/tests/textXmlTest

depth:
	python tools/testTools/test.py build/tests/depthTest

graphics:
	python tools/testTools/test.py build/tests/graphicsTest

thread:
	python tools/testTools/test.py build/tests/threadTest

vector:
	python tools/testTools/test.py build/tests/vectorEngineTest

list:
	python tools/testTools/test.py build/tests/listTest

switch:
	python tools/testTools/test.py build/tests/switchTest

svg:
	python tools/testTools/test.py build/tests/svgTest

crop:
	python tools/testTools/test.py build/tests/cropRegionTest

win2:
	python tools/testTools/test.py build/tests/multiWindowTest

test-all:
	python tools/testTools/test.py