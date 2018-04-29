#include "WrittenBox.h"






WrittenBox::WrittenBox(float x_, float y_, float width_, float height_, int fontSize_, string text_, const char * fontPath, const char * fontColor)
{
	this->x = x_;
	this->y = y_;
	this->width = width_;
	this->height = height_;
	this->text = text_;
	this->bitmap = al_create_bitmap(width_, height_);
	this->font = al_load_font(fontPath, -fontSize_, ALLEGRO_ALIGN_CENTRE);
	this->fontColor = al_color_name(fontColor);

	ALLEGRO_BITMAP * c = al_get_backbuffer(al_get_current_display());
	al_set_target_bitmap(this->bitmap);

	al_clear_to_color(al_map_rgba(0, 0, 0, 0));

	al_draw_text(this->font, this->fontColor, this->width / 2.0, this->height / 2.0, ALLEGRO_ALIGN_CENTRE, this->text.c_str());

	al_set_target_bitmap(c);

}

WrittenBox::~WrittenBox()
{
	if (this->bitmap)
		al_destroy_bitmap(this->bitmap);
	if (this->font)
		al_destroy_font(this->font);
}

void WrittenBox::draw()
{
	al_draw_bitmap(this->bitmap,this->x,this->y,0);
}
