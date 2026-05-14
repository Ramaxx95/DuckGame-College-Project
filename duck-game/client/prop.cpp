#include "prop.h"

Prop::Prop(std::unique_ptr<Sprite> sprite): prop_sprite(std::move(sprite)) {}

Prop::~Prop() {}
