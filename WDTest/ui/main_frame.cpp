#include "ui_common.h"

extern void menu_bar_init(Wnd* main_frame);
extern void text_editor_init(Wnd* main_frame);
extern void status_bar_init(Wnd* main_frame);


// Style init method 1: inherit the Style class and initialize the styles.
struct MainFrameStyle :public Style {
	MainFrameStyle() {
		size = { 1000px, 500px };
		min_size = { 300px, 200px };
	}
}main_frame_style;

Wnd* main_frame;

bool main_frame_handler(Msg msg, void* para) {

}

void main_frame_init() {
	// Create message will be sent before this function returns.
	main_frame = Wnd::Create(desktop, &main_frame_style, main_frame_handler);

	menu_bar_init(main_frame);
	text_editor_init(main_frame);
	status_bar_init(main_frame);

	main_frame->Show();
}

void main_frame_final() {
	// main_frame->Destroy(); // No need, it will be destroyed automatically.
}