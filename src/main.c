#include <pebble.h>

static Window *s_main_window;
static BitmapLayer *s_hour_layer_1;
static BitmapLayer *s_hour_layer_2;
static BitmapLayer *s_minute_layer_1;
static BitmapLayer *s_minute_layer_2;
static TextLayer *s_spacer_layer_top;
static BitmapLayer *s_date_layer_1;
static BitmapLayer *s_date_layer_2;
static BitmapLayer *s_month_layer;
static BitmapLayer *s_weekday_layer;
static BitmapLayer *s_battery_layer;
static TextLayer * s_spacer_layer_bottom;

// Bitmaps
// Digits
static GBitmap *s_digit_0;
static GBitmap *s_digit_1;
static GBitmap *s_digit_2;
static GBitmap *s_digit_3;
static GBitmap *s_digit_4;
static GBitmap *s_digit_5;
static GBitmap *s_digit_6;
static GBitmap *s_digit_7;
static GBitmap *s_digit_8;
static GBitmap *s_digit_9;

// Dates
static GBitmap *s_date_0;
static GBitmap *s_date_1_left;
static GBitmap *s_date_1_right;
static GBitmap *s_date_2;
static GBitmap *s_date_3;
static GBitmap *s_date_4;
static GBitmap *s_date_5;
static GBitmap *s_date_6;
static GBitmap *s_date_7;
static GBitmap *s_date_8;
static GBitmap *s_date_9;

// Months
static GBitmap *s_month_jan;
static GBitmap *s_month_feb;
static GBitmap *s_month_mar;
static GBitmap *s_month_apr;
static GBitmap *s_month_may;
static GBitmap *s_month_jun;
static GBitmap *s_month_jul;
static GBitmap *s_month_aug;
static GBitmap *s_month_sep;
static GBitmap *s_month_oct;
static GBitmap *s_month_nov;
static GBitmap *s_month_dec;

// Weekdays
static GBitmap *s_day_mon;
static GBitmap *s_day_tue;
static GBitmap *s_day_wed;
static GBitmap *s_day_thu;
static GBitmap *s_day_fri;
static GBitmap *s_day_sat;
static GBitmap *s_day_sun;

// Battery state
static GBitmap *s_battery_0;
static GBitmap *s_battery_10;
static GBitmap *s_battery_20;
static GBitmap *s_battery_30;
static GBitmap *s_battery_40;
static GBitmap *s_battery_50;
static GBitmap *s_battery_60;
static GBitmap *s_battery_70;
static GBitmap *s_battery_80;
static GBitmap *s_battery_90;
static GBitmap *s_battery_100;
static GBitmap *s_battery_charging;

// Constants
#define MISC_WIDTH 18
#define MISC_SPACING 10

void setBitmapForDigit(char digit, BitmapLayer *layer) {
	switch (digit) {
		case '0':
			bitmap_layer_set_bitmap(layer, s_digit_0);
			break;
		
		case '1':
			bitmap_layer_set_bitmap(layer, s_digit_1);
			break;
		
		case '2':
			bitmap_layer_set_bitmap(layer, s_digit_2);
			break;
		
		case '3':
			bitmap_layer_set_bitmap(layer, s_digit_3);
			break;
		
		case '4':
			bitmap_layer_set_bitmap(layer, s_digit_4);
			break;
		
		case '5':
			bitmap_layer_set_bitmap(layer, s_digit_5);
			break;
		
		case '6':
			bitmap_layer_set_bitmap(layer, s_digit_6);
			break;
		
		case '7':
			bitmap_layer_set_bitmap(layer, s_digit_7);
			break;
		
		case '8':
			bitmap_layer_set_bitmap(layer, s_digit_8);
			break;
		
		case '9':
			bitmap_layer_set_bitmap(layer, s_digit_9);
			break;
	}
}

void setBitmapForDate(char date, BitmapLayer *layer, bool isSecondDate) {
	switch (date) {
		case '0':
			bitmap_layer_set_bitmap(layer, s_date_0);
			break;
		
		case '1':
			if (isSecondDate)
				bitmap_layer_set_bitmap(layer, s_date_1_left);
			else
				bitmap_layer_set_bitmap(layer, s_date_1_right);
			break;
		
		case '2':
			bitmap_layer_set_bitmap(layer, s_date_2);
			break;
		
		case '3':
			bitmap_layer_set_bitmap(layer, s_date_3);
			break;
		
		case '4':
			bitmap_layer_set_bitmap(layer, s_date_4);
			break;
		
		case '5':
			bitmap_layer_set_bitmap(layer, s_date_5);
			break;
		
		case '6':
			bitmap_layer_set_bitmap(layer, s_date_6);
			break;
		
		case '7':
			bitmap_layer_set_bitmap(layer, s_date_7);
			break;
		
		case '8':
			bitmap_layer_set_bitmap(layer, s_date_8);
			break;
		
		case '9':
			bitmap_layer_set_bitmap(layer, s_date_9);
			break;
	}
}

void setBitmapForDay(char day, BitmapLayer *layer) {
	switch (day) {
		case '0':
			bitmap_layer_set_bitmap(layer, s_day_sun);
			break;
		
		case '1':
			bitmap_layer_set_bitmap(layer, s_day_mon);
			break;
		
		case '2':
			bitmap_layer_set_bitmap(layer, s_day_tue);
			break;
		
		case '3':
			bitmap_layer_set_bitmap(layer, s_day_wed);
			break;
		
		case '4':
			bitmap_layer_set_bitmap(layer, s_day_thu);
			break;
		
		case '5':
			bitmap_layer_set_bitmap(layer, s_day_fri);
			break;
		
		case '6':
			bitmap_layer_set_bitmap(layer, s_day_sat);
			break;
	}
}

static void update_time() {
	// Get a tm structure
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);
	
	// Write the current hours and minutes into a buffer
	static char s_buffer[12];
	strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H%M%d%m%w" : "%I%M%d%m%w", tick_time);
	
	bool skipFirstHourDigit = s_buffer[0] == '0';
	
	// First hour digit if needed
	if (!skipFirstHourDigit)
		setBitmapForDigit(s_buffer[0], s_hour_layer_1);
	else
		bitmap_layer_set_bitmap(s_hour_layer_1, NULL);
	
	// Second hour digit
	setBitmapForDigit(s_buffer[1], s_hour_layer_2);
	
	// First minute digit
	setBitmapForDigit(s_buffer[2], s_minute_layer_1);
	
	// Second minute digit
	setBitmapForDigit(s_buffer[3], s_minute_layer_2);
	
	// First date digit
	setBitmapForDate(s_buffer[4], s_date_layer_1, false);
	
	// Second date digit
	setBitmapForDate(s_buffer[5], s_date_layer_2, true);
	
	char month[3] = {s_buffer[6], s_buffer[7], '\0'};
	
	// Month
	if (!strcmp(month, "01"))
		bitmap_layer_set_bitmap(s_month_layer, s_month_jan);
	else if (!strcmp(month, "02"))
		bitmap_layer_set_bitmap(s_month_layer, s_month_feb);
	else if (!strcmp(month, "03"))
		bitmap_layer_set_bitmap(s_month_layer, s_month_mar);
	else if (!strcmp(month, "04"))
		bitmap_layer_set_bitmap(s_month_layer, s_month_apr);
	else if (!strcmp(month, "05"))
		bitmap_layer_set_bitmap(s_month_layer, s_month_may);
	else if (!strcmp(month, "06"))
		bitmap_layer_set_bitmap(s_month_layer, s_month_jun);
	else if (!strcmp(month, "07"))
		bitmap_layer_set_bitmap(s_month_layer, s_month_jul);
	else if (!strcmp(month, "08"))
		bitmap_layer_set_bitmap(s_month_layer, s_month_aug);
	else if (!strcmp(month, "09"))
		bitmap_layer_set_bitmap(s_month_layer, s_month_sep);
	else if (!strcmp(month, "10"))
		bitmap_layer_set_bitmap(s_month_layer, s_month_oct);
	else if (!strcmp(month, "11"))
		bitmap_layer_set_bitmap(s_month_layer, s_month_nov);
	else if (!strcmp(month, "12"))
		bitmap_layer_set_bitmap(s_month_layer, s_month_dec);
	
	// Weekday
	setBitmapForDay(s_buffer[8], s_weekday_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	update_time();
}

// Battery handler
static void battery_handler(BatteryChargeState charge_state) {
	if (charge_state.is_charging) {
		bitmap_layer_set_bitmap(s_battery_layer, s_battery_charging);
	} else {
		if (charge_state.charge_percent > 95)
			bitmap_layer_set_bitmap(s_battery_layer, s_battery_100);
		else if (charge_state.charge_percent > 85)
			bitmap_layer_set_bitmap(s_battery_layer, s_battery_90);
		else if (charge_state.charge_percent > 75)
			bitmap_layer_set_bitmap(s_battery_layer, s_battery_80);
		else if (charge_state.charge_percent > 65)
			bitmap_layer_set_bitmap(s_battery_layer, s_battery_70);
		else if (charge_state.charge_percent > 55)
			bitmap_layer_set_bitmap(s_battery_layer, s_battery_60);
		else if (charge_state.charge_percent > 45)
			bitmap_layer_set_bitmap(s_battery_layer, s_battery_50);
		else if (charge_state.charge_percent > 35)
			bitmap_layer_set_bitmap(s_battery_layer, s_battery_40);
		else if (charge_state.charge_percent > 25)
			bitmap_layer_set_bitmap(s_battery_layer, s_battery_30);
		else if (charge_state.charge_percent > 15)
			bitmap_layer_set_bitmap(s_battery_layer, s_battery_20);
		else if (charge_state.charge_percent > 5)
			bitmap_layer_set_bitmap(s_battery_layer, s_battery_10);
		else
			bitmap_layer_set_bitmap(s_battery_layer, s_battery_0);
	}
}

static void main_window_load(Window *window) {
	// Get information about the Window
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);
		
	// Load bitmaps
	// Digits
	s_digit_0 = gbitmap_create_with_resource(RESOURCE_ID_DIGIT_0);
	s_digit_1 = gbitmap_create_with_resource(RESOURCE_ID_DIGIT_1);
	s_digit_2 = gbitmap_create_with_resource(RESOURCE_ID_DIGIT_2);
	s_digit_3 = gbitmap_create_with_resource(RESOURCE_ID_DIGIT_3);
	s_digit_4 = gbitmap_create_with_resource(RESOURCE_ID_DIGIT_4);
	s_digit_5 = gbitmap_create_with_resource(RESOURCE_ID_DIGIT_5);
	s_digit_6 = gbitmap_create_with_resource(RESOURCE_ID_DIGIT_6);
	s_digit_7 = gbitmap_create_with_resource(RESOURCE_ID_DIGIT_7);
	s_digit_8 = gbitmap_create_with_resource(RESOURCE_ID_DIGIT_8);
	s_digit_9 = gbitmap_create_with_resource(RESOURCE_ID_DIGIT_9);
	
	// Dates
	s_date_0 = gbitmap_create_with_resource(RESOURCE_ID_DATE_0);
	s_date_1_left = gbitmap_create_with_resource(RESOURCE_ID_DATE_1_LEFT);
	s_date_1_right = gbitmap_create_with_resource(RESOURCE_ID_DATE_1_RIGHT);
	s_date_2 = gbitmap_create_with_resource(RESOURCE_ID_DATE_2);
	s_date_3 = gbitmap_create_with_resource(RESOURCE_ID_DATE_3);
	s_date_4 = gbitmap_create_with_resource(RESOURCE_ID_DATE_4);
	s_date_5 = gbitmap_create_with_resource(RESOURCE_ID_DATE_5);
	s_date_6 = gbitmap_create_with_resource(RESOURCE_ID_DATE_6);
	s_date_7 = gbitmap_create_with_resource(RESOURCE_ID_DATE_7);
	s_date_8 = gbitmap_create_with_resource(RESOURCE_ID_DATE_8);
	s_date_9 = gbitmap_create_with_resource(RESOURCE_ID_DATE_9);
	
	// Months
	s_month_jan = gbitmap_create_with_resource(RESOURCE_ID_MONTH_JAN);
	s_month_feb = gbitmap_create_with_resource(RESOURCE_ID_MONTH_FEB);
	s_month_mar = gbitmap_create_with_resource(RESOURCE_ID_MONTH_MAR);
	s_month_apr = gbitmap_create_with_resource(RESOURCE_ID_MONTH_APR);
	s_month_may = gbitmap_create_with_resource(RESOURCE_ID_MONTH_MAY);
	s_month_jun = gbitmap_create_with_resource(RESOURCE_ID_MONTH_JUN);
	s_month_jul = gbitmap_create_with_resource(RESOURCE_ID_MONTH_JUL);
	s_month_aug = gbitmap_create_with_resource(RESOURCE_ID_MONTH_AUG);
	s_month_sep = gbitmap_create_with_resource(RESOURCE_ID_MONTH_SEP);
	s_month_oct = gbitmap_create_with_resource(RESOURCE_ID_MONTH_OCT);
	s_month_nov = gbitmap_create_with_resource(RESOURCE_ID_MONTH_NOV);
	s_month_dec = gbitmap_create_with_resource(RESOURCE_ID_MONTH_DEC);
	
	// Weekdays
	s_day_mon = gbitmap_create_with_resource(RESOURCE_ID_WEEKDAY_MON);
	s_day_tue = gbitmap_create_with_resource(RESOURCE_ID_WEEKDAY_TUE);
	s_day_wed = gbitmap_create_with_resource(RESOURCE_ID_WEEKDAY_WED);
	s_day_thu = gbitmap_create_with_resource(RESOURCE_ID_WEEKDAY_THU);
	s_day_fri = gbitmap_create_with_resource(RESOURCE_ID_WEEKDAY_FRI);
	s_day_sat = gbitmap_create_with_resource(RESOURCE_ID_WEEKDAY_SAT);
	s_day_sun = gbitmap_create_with_resource(RESOURCE_ID_WEEKDAY_SUN);
	
	// Battery state
	s_battery_0 = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_0);
	s_battery_10 = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_10);
	s_battery_20 = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_20);
	s_battery_30 = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_30);
	s_battery_40 = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_40);
	s_battery_50 = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_50);
	s_battery_60 = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_60);
	s_battery_70 = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_70);
	s_battery_80 = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_80);
	s_battery_90 = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_90);
	s_battery_100 = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_100);
	s_battery_charging = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_CHARGING);
	
	// Create single digit layers
	int digit_width = (bounds.size.w - MISC_WIDTH) / 2;
	int digit_height = bounds.size.h / 2;
	
	s_hour_layer_1 = bitmap_layer_create(GRect(0, 0, digit_width, digit_height));
	s_hour_layer_2 = bitmap_layer_create(GRect(digit_width, 0, digit_width, digit_height));
	s_minute_layer_1 = bitmap_layer_create(GRect(0, digit_height, digit_width, digit_height));
	s_minute_layer_2 = bitmap_layer_create(GRect(digit_width, digit_height, digit_width, digit_height));
	
	// Create single date layers
	int date_origin = MISC_SPACING;
	int date_height = 9;
	
	s_date_layer_1 = bitmap_layer_create(GRect(bounds.size.w - MISC_WIDTH, date_origin + date_height, MISC_WIDTH, date_height));
	s_date_layer_2 = bitmap_layer_create(GRect(bounds.size.w - MISC_WIDTH, date_origin, MISC_WIDTH, date_height));
	
	// Create month layer
	int month_origin = date_origin + 2 * date_height;
	int month_height = 36;
	
	s_month_layer = bitmap_layer_create(GRect(bounds.size.w - MISC_WIDTH, month_origin, MISC_WIDTH, month_height));
	
	// Create weekday layer
	int weekday_origin = month_origin + month_height;
	int weekday_height = 36;
	
	s_weekday_layer = bitmap_layer_create(GRect(bounds.size.w - MISC_WIDTH, weekday_origin, MISC_WIDTH, weekday_height));
	
	// Create battery layer
	int battery_origin = weekday_origin + weekday_height;
	int battery_height = 20;
	
	s_battery_layer = bitmap_layer_create(GRect(bounds.size.w - MISC_WIDTH, battery_origin, MISC_WIDTH, battery_height));
	
	// Create spacer layers
	int bottom_spacer_origin = battery_origin + battery_height;
	int bottom_spacer_height = bounds.size.h - bottom_spacer_origin;
	
	s_spacer_layer_top = text_layer_create(GRect(bounds.size.w - MISC_WIDTH, 0, MISC_WIDTH, MISC_SPACING));
	s_spacer_layer_bottom = text_layer_create(GRect(bounds.size.w - MISC_WIDTH, bottom_spacer_origin, MISC_WIDTH, bottom_spacer_height));
	
	// Improve the layout to be more like a watchface
	// Set background color
	GColor background_color = GColorBlack;
	bitmap_layer_set_background_color(s_hour_layer_1, background_color);
	bitmap_layer_set_background_color(s_hour_layer_2, background_color);
	bitmap_layer_set_background_color(s_minute_layer_1, background_color);
	bitmap_layer_set_background_color(s_minute_layer_2, background_color);
	text_layer_set_background_color(s_spacer_layer_top, background_color);
	bitmap_layer_set_background_color(s_date_layer_1, background_color);
	bitmap_layer_set_background_color(s_date_layer_2, background_color);
	bitmap_layer_set_background_color(s_month_layer, background_color);
	bitmap_layer_set_background_color(s_weekday_layer, background_color);
	bitmap_layer_set_background_color(s_battery_layer, background_color);
	text_layer_set_background_color(s_spacer_layer_bottom, background_color);
	
	// Set alignment
	bitmap_layer_set_alignment(s_hour_layer_1, GAlignCenter);
	bitmap_layer_set_alignment(s_hour_layer_2, GAlignCenter);
	bitmap_layer_set_alignment(s_minute_layer_1, GAlignCenter);
	bitmap_layer_set_alignment(s_minute_layer_2, GAlignCenter);
	bitmap_layer_set_alignment(s_date_layer_1, GAlignTop);
	bitmap_layer_set_alignment(s_date_layer_2, GAlignTop);
	bitmap_layer_set_alignment(s_month_layer, GAlignTop);
	bitmap_layer_set_alignment(s_weekday_layer, GAlignTop);
	bitmap_layer_set_alignment(s_battery_layer, GAlignTop);
	
	// Add time layer as child layers to the Window’s root layer
	layer_add_child(window_layer, bitmap_layer_get_layer(s_hour_layer_1));
	layer_add_child(window_layer, bitmap_layer_get_layer(s_hour_layer_2));
	layer_add_child(window_layer, bitmap_layer_get_layer(s_minute_layer_1));
	layer_add_child(window_layer, bitmap_layer_get_layer(s_minute_layer_2));
	
	// Add misc layers child layers to the Window’s root layer
	layer_add_child(window_layer, text_layer_get_layer(s_spacer_layer_top));
	layer_add_child(window_layer, bitmap_layer_get_layer(s_date_layer_1));
	layer_add_child(window_layer, bitmap_layer_get_layer(s_date_layer_2));
	layer_add_child(window_layer, bitmap_layer_get_layer(s_month_layer));
	layer_add_child(window_layer, bitmap_layer_get_layer(s_weekday_layer));
	layer_add_child(window_layer, bitmap_layer_get_layer(s_battery_layer));
	layer_add_child(window_layer, text_layer_get_layer(s_spacer_layer_bottom));
}

static void main_window_unload(Window *window) {
	// Destroy time layers
	bitmap_layer_destroy(s_hour_layer_1);
	bitmap_layer_destroy(s_hour_layer_2);
	bitmap_layer_destroy(s_minute_layer_1);
	bitmap_layer_destroy(s_minute_layer_2);
	
	// Destroy misc layers
	text_layer_destroy(s_spacer_layer_top);
	bitmap_layer_destroy(s_date_layer_1);
	bitmap_layer_destroy(s_date_layer_2);
	bitmap_layer_destroy(s_month_layer);
	bitmap_layer_destroy(s_weekday_layer);
	bitmap_layer_destroy(s_battery_layer);
	text_layer_destroy(s_spacer_layer_bottom);
	
	// Unload bitmaps
	gbitmap_destroy(s_digit_0);
	gbitmap_destroy(s_digit_1);
	gbitmap_destroy(s_digit_2);
	gbitmap_destroy(s_digit_3);
	gbitmap_destroy(s_digit_4);
	gbitmap_destroy(s_digit_5);
	gbitmap_destroy(s_digit_6);
	gbitmap_destroy(s_digit_7);
	gbitmap_destroy(s_digit_8);
	gbitmap_destroy(s_digit_9);
	gbitmap_destroy(s_date_0);
	gbitmap_destroy(s_date_1_left);
	gbitmap_destroy(s_date_1_right);
	gbitmap_destroy(s_date_2);
	gbitmap_destroy(s_date_3);
	gbitmap_destroy(s_date_4);
	gbitmap_destroy(s_date_5);
	gbitmap_destroy(s_date_6);
	gbitmap_destroy(s_date_7);
	gbitmap_destroy(s_date_8);
	gbitmap_destroy(s_date_9);
	gbitmap_destroy(s_month_jan);
	gbitmap_destroy(s_month_feb);
	gbitmap_destroy(s_month_mar);
	gbitmap_destroy(s_month_apr);
	gbitmap_destroy(s_month_may);
	gbitmap_destroy(s_month_jun);
	gbitmap_destroy(s_month_jul);
	gbitmap_destroy(s_month_aug);
	gbitmap_destroy(s_month_sep);
	gbitmap_destroy(s_month_oct);
	gbitmap_destroy(s_month_nov);
	gbitmap_destroy(s_month_dec);
	gbitmap_destroy(s_day_mon);
	gbitmap_destroy(s_day_tue);
	gbitmap_destroy(s_day_wed);
	gbitmap_destroy(s_day_thu);
	gbitmap_destroy(s_day_fri);
	gbitmap_destroy(s_day_sat);
	gbitmap_destroy(s_day_sun);
	gbitmap_destroy(s_battery_0);
	gbitmap_destroy(s_battery_10);
	gbitmap_destroy(s_battery_20);
	gbitmap_destroy(s_battery_30);
	gbitmap_destroy(s_battery_40);
	gbitmap_destroy(s_battery_50);
	gbitmap_destroy(s_battery_60);
	gbitmap_destroy(s_battery_70);
	gbitmap_destroy(s_battery_80);
	gbitmap_destroy(s_battery_90);
	gbitmap_destroy(s_battery_100);
	gbitmap_destroy(s_battery_charging);
}

static void init() {	
	// Create main Window element and assign to pointer
	s_main_window = window_create();
	
	// Set handlers to manage the elements inside the Window
	window_set_window_handlers(s_main_window, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload
	});
	
	// Show the Window on the watch, with animated = true
	window_stack_push(s_main_window, true);
	
	// Register with TickTimerService
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
	
	// Register with BatteryStateService
	battery_state_service_subscribe(battery_handler);
	
	// Make sure the time is displayed from the start
	update_time();
	
	// And the battery is shown from the start
	battery_handler(battery_state_service_peek());
}

static void deinit() {
	// Destroy Window
	window_destroy(s_main_window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}
