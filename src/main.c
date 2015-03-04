#include <pebble.h>

Window *my_window;
TextLayer *text_layer;

#define TOTAL_FRAMES 22
	
static const uint32_t timeout_ms = 120;

static int current_frame = 0;
	
BitmapLayer *background_layer, *eye_layer;
static Layer *anim_layer;
GBitmap *current_image;

const int ANIM_IMAGE_RESOURCE_IDS[] = {
	RESOURCE_ID_IMAGE_00,
  RESOURCE_ID_IMAGE_01,
  RESOURCE_ID_IMAGE_02,
  RESOURCE_ID_IMAGE_03,
  RESOURCE_ID_IMAGE_04,
  RESOURCE_ID_IMAGE_05,
  RESOURCE_ID_IMAGE_06,
  RESOURCE_ID_IMAGE_07,
  RESOURCE_ID_IMAGE_08,
  RESOURCE_ID_IMAGE_09,
  RESOURCE_ID_IMAGE_10,
	RESOURCE_ID_IMAGE_11,
  RESOURCE_ID_IMAGE_12,
  RESOURCE_ID_IMAGE_13,
  RESOURCE_ID_IMAGE_14,
  RESOURCE_ID_IMAGE_15,
  RESOURCE_ID_IMAGE_16,
  RESOURCE_ID_IMAGE_17,
  RESOURCE_ID_IMAGE_18,
  RESOURCE_ID_IMAGE_19,
  RESOURCE_ID_IMAGE_20,
	RESOURCE_ID_IMAGE_21
};


static AppTimer *timer;

void woosh_woosh() {
	gbitmap_destroy(current_image);
	if(current_frame >= TOTAL_FRAMES) {
		current_frame = 0;
	}
	current_image = gbitmap_create_with_resource(ANIM_IMAGE_RESOURCE_IDS[current_frame]);
	bitmap_layer_set_bitmap(eye_layer, current_image);
	current_frame++;
}

static void update_image_layer(Layer *layer, GContext* ctx) {
	woosh_woosh();
}

static void timer_callback(void *context) {
  layer_mark_dirty(anim_layer);
  timer = app_timer_register(timeout_ms, timer_callback, NULL);
}


void handle_init(void) {
  my_window = window_create();
  window_set_background_color(my_window, GColorBlack);
  window_set_fullscreen(my_window, true);
	
	light_enable(true);
	
	GBitmap *background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
	background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(background_layer, background);
  layer_add_child(window_get_root_layer(my_window), bitmap_layer_get_layer(background_layer));	
	
  anim_layer = layer_create(GRect(42, 74, 67, 11));
  layer_set_update_proc(anim_layer, update_image_layer);
  layer_add_child(window_get_root_layer(my_window), anim_layer);
	
  eye_layer = bitmap_layer_create(GRect(0, 0, 67, 11));
  layer_add_child(anim_layer, bitmap_layer_get_layer(eye_layer));
	
  //text_layer = text_layer_create(GRect(0, 0, 144, 20));
  window_stack_push(my_window, true);
	
	timer = app_timer_register(timeout_ms, timer_callback, NULL);
}

void handle_deinit(void) {
  text_layer_destroy(text_layer);
  window_destroy(my_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
