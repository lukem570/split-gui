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

test-all:
	python tools/testTools/test.py