#include "test.h"
#include <stdio.h>
#include "global.h"
#include "object.h"
#include "main_memory.h"

TestObject * testCreate(void) {
	GameObjectData data;

	data.test_object = (TestObject) {
		.memory_link = NULL,
		.update_handle = updateHandleCreateEmpty()
	};

	GameObject test_object = gameObjectCreate(TEST_OBJECT, data);

	GameObjectListLink * link = mainMemoryAddObject(&MAIN_MEMORY, test_object);

	testSetupRoutine(link);

	return & link->object.data.test_object;
}

void testDestroy(TestObject * object) {
	updateHandleRemove(&object->update_handle);
	mainMemoryRemoveObject(&MAIN_MEMORY, object->memory_link);
}

/** Updating callbacks **/
void testUpdateDeclaration(void * data) {}
void testUpdateApplication(void * data) {
	TestObject * object = data;
	char input = 'n';
	printf("Objet %4p : dois-je mourir ? (o/n)\n", (void *) object->memory_link);
	input = getc(stdin);
	getc(stdin);
	if( input == 'o' || input == 'O' || input == 'y' || input == 'Y' )
		testDestroy(object);
	else	
		updateRegisterAdd(&UPDATE_REGISTER, &object->update_handle, 0);
}

/** Memory setup function **/
void testSetupRoutine(GameObjectListLink * link) {
	TestObject * test_object = &link->object.data.test_object;
	test_object->memory_link = link;

	updateHandleInit(
		&test_object->update_handle,
		 test_object,
		testUpdateDeclaration,
		testUpdateApplication);

	updateRegisterAdd(&UPDATE_REGISTER, &test_object->update_handle, 0);
}

/** Test initialisation functions **/
void testListManipulationMain() {
	globalInit();

	for(int k = 0; k < 10; k++)
		testCreate();

	char input;
	//for(int k; k < 40; k++) {
	for(;;) {
		printf("[%5ld] :\n", UPDATE_REGISTER.clock);
		updateRegisterUpdate(&UPDATE_REGISTER);

		/*GameObject * first_object = mainMemoryAccessObject(&MAIN_MEMORY, 0);
		test_object * first_test_object = &first_object->data.test_object;

		printf("Original :\n");
		test_objectPrint(first_test_object);
		printf("\n");*/


		input = getc(stdin);
		if(input == 'x') break;
		if(input == '\n') continue;
		else while(getc(stdin) != '\n');
	}

	globalFree();
}
