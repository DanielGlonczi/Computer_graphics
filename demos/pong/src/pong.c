#include "pong.h"
#include <math.h>

void init_pong(Pong* pong, int width, int height)
{
    pong->width = width;
    pong->height = height;
    init_pad(&(pong->left_pad), 0, height, RED_THEME);
    init_pad(&(pong->right_pad), width - 50, height, GREEN_THEME);
    init_ball(&(pong->ball), width / 2, height / 2);
    pong->left_score = 0;
    pong->right_score = 0;
}

void update_pong(Pong* pong, double time)
{
    update_pad(&(pong->left_pad), time);
    update_pad(&(pong->right_pad), time);
    update_ball(&(pong->ball), time);
    bounce_ball(pong);
}

void render_pong(Pong* pong)
{
    render_pad(&(pong->left_pad));
    render_pad(&(pong->right_pad));
    render_ball(&(pong->ball));
}

void set_left_pad_position(Pong* pong, float position)
{
    set_pad_position(&(pong->left_pad), position);
}

void set_left_pad_speed(Pong* pong, float speed)
{
    set_pad_speed(&(pong->left_pad), speed);
}

void set_right_pad_position(Pong* pong, float position)
{
    set_pad_position(&(pong->right_pad), position);
}

void set_right_pad_speed(Pong* pong, float speed)
{
    set_pad_speed(&(pong->right_pad), speed);
}

void bounce_ball(Pong* pong)
{
    Ball* b = &(pong->ball);
    Pad* lp = &(pong->left_pad);
    Pad* rp = &(pong->right_pad);

    if (b->x - b->radius < lp->x + lp->width) {
        if (b->y + b->radius >= lp->y && b->y - b->radius <= lp->y + lp->height) {
            b->x = lp->x + lp->width + b->radius;
            b->speed_x = fabs(b->speed_x);
            {
                float pad_center = lp->y + lp->height * 0.5f;
                float rel = (b->y - pad_center) / (lp->height * 0.5f);
                b->angular_speed += rel * 180.0f;
            }
        } else {
            pong->right_score++;
            init_ball(b, pong->width / 2, pong->height / 2);
            b->speed_x = 200; 
        }
    }

    if (b->x + b->radius > rp->x) {
        if (b->y + b->radius >= rp->y && b->y - b->radius <= rp->y + rp->height) {
            b->x = rp->x - b->radius;
            b->speed_x = -fabs(b->speed_x);
            {
                float pad_center = rp->y + rp->height * 0.5f;
                float rel = (b->y - pad_center) / (rp->height * 0.5f);
                b->angular_speed += rel * 180.0f;
            }
        } else {
            pong->left_score++;
            init_ball(b, pong->width / 2, pong->height / 2);
            b->speed_x = -200;  
        }
    }

    
    if (b->y - b->radius < 0) {
        b->y = b->radius;
        b->speed_y *= -1;
        b->angular_speed *= -1;
    }
    if (b->y + b->radius > pong->height) {
        b->y = pong->height - b->radius;
        b->speed_y *= -1;
        b->angular_speed *= -1;
    }
}
