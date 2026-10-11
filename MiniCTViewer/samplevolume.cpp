#include "samplevolume.h"

#include <vtkType.h>

vtkSmartPointer<vtkImageData> createSampleVolume()
{
    auto volume = vtkSmartPointer<vtkImageData>::New();

    volume->SetDimensions(64, 64, 64);
    volume->SetSpacing(1.0, 1.0, 1.0);
    volume->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

    for (int z = 0; z < 64; ++z) {
        for (int y = 0; y < 64; ++y) {
            for (int x = 0; x < 64; ++x) {
                double dx = x - 31.5;
                double dy = y - 31.5;
                double dz = z - 31.5;

                double distanceSquared =
                    dx * dx + dy * dy + dz * dz;

                double value =
                    (distanceSquared <= 24.0 * 24.0) ? 220.0 : 20.0;

                volume->SetScalarComponentFromDouble(
                    x, y, z, 0, value
                    );
            }
        }
    }

    return volume;
}