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

test-all:
	python tools/testTools/test.py