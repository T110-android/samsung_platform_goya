/*
 * Copyright 2010, The Android Open Source Project
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

//SAMSUNG CHANGE - MPSG6020 >>
#include <limits>
#include <math.h>
//SAMSUNG CHANGE - MPSG6020 <<

#include "Connection.h"

//SAMSUNG CHANGE - MPSG6020 >>
#include "Event.h"
#include "EventHandler.h"
#include "EventListener.h"
#include "EventNames.h"
#include "EventQueue.h"
#include "ExceptionCode.h"
#include "Document.h"
//SAMSUNG CHANGE - MPSG6020 <<

#include "NetworkStateNotifier.h"

namespace WebCore {

Connection::ConnectionType Connection::type() const
{
    return networkStateNotifier().type();
}

//SAMSUNG CHANGE - MPSG6020 >>

double Connection::bandwidth() const
{
   if (!networkStateNotifier().onLine())
   	return 0;
   else
   	return std::numeric_limits<double>::infinity();  //till the necessary network api to measure bandwidth becomes available, we return infinity as per the specifications 
}


bool Connection::metered() const
{
   if (!networkStateNotifier().onLine())
   	return 0;
   else
   	return 0;   // ideally we should have some means of setting a user preference on the wifi side if the connection is pay-per-use etc, but for the moment, as we don't know, we will return false as default
   	
}


ScriptExecutionContext* Connection::scriptExecutionContext() const
{
    return ActiveDOMObject::scriptExecutionContext();
}


void Connection::fireEvent(const AtomicString& type)
{
    dispatchEvent(Event::create(type, false, false));
}

	bool Connection::hasPendingActivity() const
	{
    return (ActiveDOMObject::hasPendingActivity());
	}

	bool Connection::canSuspend() const
	{
    return false;
	}

	void Connection::stop() const
	{
    //Not Implemented
	}

//SAMSUNG CHANGE - MPSG6020<<
	
};
