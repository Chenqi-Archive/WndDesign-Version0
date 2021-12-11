#include "ui_common.h"

Style menu_bar_style;
Style close_btn_style;
Style menu_style;

extern Wnd* main_frame;
Wnd* menu_bar;

Button* close_btn;

Button* file_menu;

bool close_btn_on_click() {
	main_frame->Destroy();
}

bool file_menu_on_click() {

}

bool menu_bar_handler(Msg msg, void* para) {
	static bool has_mouse_down = false;
	if (msg == Msg::LeftDown) {
		has_mouse_down = true;
	}
	if (msg == Msg::LeftUp) {
		has_mouse_down = false;
	}
	if (msg == Msg::MouseMove) {
		
	}
}

void menu_bar_init(Wnd* main_frame) {
	// Style init method 2: change the default settings of a Style object.
	menu_bar_style.size = { 100pct, 20px };
	menu_bar = Wnd::Create(main_frame, &menu_bar_style, menu_bar_handler);

	close_btn_style.size = { 20px, 20px };
	close_btn = Button::Create(menu_bar, &close_btn_style, close_btn_on_click);

	file_menu = Button::Create(menu_bar, &menu_style, )
}