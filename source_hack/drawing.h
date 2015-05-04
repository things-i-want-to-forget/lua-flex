#ifndef DRAWING_H
#define DRAWING_H

class Vector;

extern bool ScreenTransform(const Vector &point, Vector &screen);
extern bool WorldToScreen(const Vector &origin, Vector &screen);

#endif // DRAWING_H