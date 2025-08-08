#pragma once

//#include <sdkddkver.h>

#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <string>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "Message.h"
#include "flatbuffers/flatbuffers.h"
#include "protocol_generated.h"

using namespace std;
using boost::asio::ip::tcp;


