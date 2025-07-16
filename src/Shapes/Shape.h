#pragma once

class Shape
{
public:
    virtual void Init() = 0;
    virtual void Rotate() = 0;
    void IncrementRotation()
    {
        mCurrentRotationDegrees = (mCurrentRotationDegrees + 90) % 360;
    }

    int mCurrentRotationDegrees = 0;
    bool isRotated = false;
};
