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

// include the required headers
#include "SingleThreadScheduler.h"
#include "ActorManager.h"
#include "ActorInstance.h"
#include "Attachment.h"
#include "WaveletCache.h"
#include "EMotionFXManager.h"


namespace EMotionFX
{
    // constructor
    SingleThreadScheduler::SingleThreadScheduler()
        : ActorUpdateScheduler()
    {
    }


    // destructor
    SingleThreadScheduler::~SingleThreadScheduler()
    {
    }


    // create
    SingleThreadScheduler* SingleThreadScheduler::Create()
    {
        return new SingleThreadScheduler();
    }


    // execute the schedule
    void SingleThreadScheduler::Execute(float timePassedInSeconds)
    {
        const ActorManager& actorManager = GetActorManager();

        // reset stats
        mNumUpdated.SetValue(0);
        mNumVisible.SetValue(0);
        mNumSampled.SetValue(0);

        // propagate root actor instance visibility to their attachments
        const uint32 numRootActorInstances = GetActorManager().GetNumRootActorInstances();
        for (uint32 i = 0; i < numRootActorInstances; ++i)
        {
            ActorInstance* rootInstance = actorManager.GetRootActorInstance(i);
            if (rootInstance->GetIsEnabled() == false)
            {
                continue;
            }

            rootInstance->RecursiveSetIsVisible(rootInstance->GetIsVisible());
        }

        /*  // make sure parents of attachments are updated as well
            const uint32 numActorInstances = actorManager.GetNumActorInstances();
            for (uint32 i=0; i<numActorInstances; ++i)
            {
                ActorInstance* actorInstance = actorManager.GetActorInstance(i);
                if (actorInstance->GetIsVisible())
                    actorInstance->RecursiveSetIsVisibleTowardsRoot( true );
            }*/

        // process all root actor instances, and execute them and their attachments
        for (uint32 i = 0; i < numRootActorInstances; ++i)
        {
            ActorInstance* rootActorInstance = actorManager.GetRootActorInstance(i);
            if (rootActorInstance->GetIsEnabled() == false)
            {
                continue;
            }

            RecursiveExecuteActorInstance(rootActorInstance, timePassedInSeconds);
        }
    }


    // execute the actor instance, and its attachments
    void SingleThreadScheduler::RecursiveExecuteActorInstance(ActorInstance* actorInstance, float timePassedInSeconds)
    {
        actorInstance->SetThreadIndex(0);

        mNumUpdated.Increment();

        const bool isVisible = actorInstance->GetIsVisible();

        // check if we want to sample motions
        bool sampleMotions = false;
        actorInstance->SetMotionSamplingTimer(actorInstance->GetMotionSamplingTimer() + timePassedInSeconds);
        if (actorInstance->GetMotionSamplingTimer() >= actorInstance->GetMotionSamplingRate())
        {
            sampleMotions = true;
            actorInstance->SetMotionSamplingTimer(0.0f);

            if (isVisible)
            {
                mNumSampled.Increment();
            }
        }

        if (isVisible)
        {
            mNumVisible.Increment();
        }

        // update the transformations
        actorInstance->UpdateTransformations(timePassedInSeconds, isVisible, sampleMotions);

        // shrink the cache
        GetWaveletCache().Shrink();

        // recursively process the attachments
        const uint32 numAttachments = actorInstance->GetNumAttachments();
        for (uint32 i = 0; i < numAttachments; ++i)
        {
            ActorInstance* attachment = actorInstance->GetAttachment(i)->GetAttachmentActorInstance();
            if (attachment && attachment->GetIsEnabled())
            {
                RecursiveExecuteActorInstance(attachment, timePassedInSeconds);
            }
        }
    }
}   // namespace EMotionFX
