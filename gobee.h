#ifndef GOBEE_H
#define GOBEE_H

#include <QObject>
#include <array>
class GoBee
{
public:
    GoBee();
    int n, s, x, y, old_x, old_y;
    int time_hide = 150;
};

#endif // GOBEE_H
