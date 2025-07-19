#pragma once

class Shape
{
public:
    virtual void Init() = 0;
    virtual void Rotate() = 0;

    // If you rotate it and then rotate 3 more times your back to rotation before the first rotation
    void Undo()
    {
        for (int i = 0; i < 3; i++)
        {
            IncrementRotation();
            Rotate();
        }
    }
    void IncrementRotation()
    {
        mCurrentRotationDegrees = (mCurrentRotationDegrees + 90) % 360;
    }

    int mCurrentRotationDegrees = 0;
    bool isRotated = false;
};
