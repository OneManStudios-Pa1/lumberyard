/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/

#ifndef __MCOMMON_LOOKATCAMERA_H
#define __MCOMMON_LOOKATCAMERA_H

// include required headers
#include "Camera.h"


namespace MCommon
{
    /**
     * Look at camera.
     */
    class MCOMMON_API LookAtCamera
        : public Camera
    {
        MCORE_MEMORYOBJECTCATEGORY(LookAtCamera, MCore::MCORE_DEFAULT_ALIGNMENT, MEMCATEGORY_MCOMMON)

    public:
        enum
        {
            TYPE_ID = 0x00000002
        };

        /**
         * Default constructor.
         */
        LookAtCamera();

        /**
         * Destructor.
         */
        virtual ~LookAtCamera();

        uint32 GetType() const                                      { return TYPE_ID; }
        const char* GetTypeString() const                       { return "Look At"; }

        /**
         * Look at target.
         * @param target The target position, so where the will be camera is looking at.
         * @param up The up vector, describing the roll of the camera, where (0,1,0) would mean the camera is straight up and has no roll. (0,-1,0) would be upside down, etc.
         */
        void LookAt(const MCore::Vector3& target, const MCore::Vector3& up = MCore::Vector3( 0.0f, 1.0f, 0.0f ));

        /**
         * Update the camera transformation.
         * Recalculate the view frustum, the projection and the view matrix.
         * @param timeDelta The time (in seconds) passed since the last call.
         */
        virtual void Update(float timeDelta = 0.0f);

        /**
         * Reset all camera attributes to their default settings.
         * @param flightTime The time of the interpolated flight between the actual camera position and the reset target.
         */
        virtual void Reset(float flightTime = 0.0f);

        /**
         * Set the target position. Note that the camera needs an update after setting a new target.
         * @param[in] target The new camera target.
         */
        MCORE_INLINE void SetTarget(const MCore::Vector3& target)   { mTarget = target; }

        /**
         * Get the target position.
         * @return The current camera target.
         */
        MCORE_INLINE MCore::Vector3 GetTarget() const               { return mTarget; }

        /**
         * Set the up vector for the camera. Note that the camera needs an update after setting a new up vector.
         * @param[in] up The new camera up vector.
         */
        MCORE_INLINE void SetUp(const MCore::Vector3& up)           { mUp = up; }

        /**
         * Get the camera up vector.
         * @return The current up vector.
         */
        MCORE_INLINE MCore::Vector3 GetUp() const                   { return mUp; }

    protected:
        MCore::Vector3 mTarget; /**< The camera target. */
        MCore::Vector3 mUp;     /**< The up vector of the camera. */
    };
} // namespace MCommon


#endif
