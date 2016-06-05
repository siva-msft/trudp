/*
 * The MIT License
 *
 * Copyright 2016 Kirill Scherba <kirill@scherba.ru>.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * Created on May 31, 2016, 1:45 AM
 */

#ifndef TR_UDP_H
#define TR_UDP_H

#include "timed_queue.h"
#include "packet.h"
#include "udp.h"

#ifdef __cplusplus
extern "C" {
#endif
    
#define MAX_RETRIEVES 10
#define MAX_OUTRUNNING 10    
#define START_MIDDLE_TIME (MAX_ACK_WAIT/5) * 1000000    
    
/**
 * Data received/send callback
 */
typedef void (*trudpDataCb)(void *td, void *data, size_t data_length, void *user_data);

/**
 * Event callback
 */
typedef void (*trudpEventCb)(void *td, int event, void *data, size_t data_length, void *user_data);

/**
 * Union of TR-UDP callback
 */
typedef union trudpCb {
    
    trudpDataCb data;
    trudpDataCb send;
    trudpEventCb event;
    void *ptr;
    
} trudpCb;

/**
 * Enumeration of callback types
 */
typedef enum trudpCallbsckType {
    
    PROCESS_DATA,
    PROCESS_ACK,
    EVENT,
    SEND
            
} trudpCallbsckType;

/**
 * Trudp Data Structure
 */
typedef struct trudpData {
    
    uint32_t sendId;
    trudpTimedQueue *sendQueue;
    uint32_t triptime;
    uint32_t triptimeMiddle;
        
    uint32_t receiveExpectedId;
    trudpTimedQueue *receiveQueue;
    int outrunning_cnt; ///< Receive queue outrunning count

    trudpDataCb processDataCb;
    trudpDataCb processAckCb;
    trudpEventCb evendCb;
    trudpDataCb sendCb;
    
    void* user_data;
    
    // UDP connection depended variables
    struct sockaddr_in remaddr; // remote address
    socklen_t addrlen;          // remote address length
    int connected_f;            // connected (remote address valid)
    int fd;
    
} trudpData;

trudpData *trudpNew(void *user_data); //, trudpDataCb processDataCb, trudpDataCb sendPacketCb);
trudpCb trudpSetCallback(trudpData *td, trudpCallbsckType type, trudpCb cb);
void trudpDestroy(trudpData *td);
void trudpFree(trudpData *td);
void trudpReset(trudpData *td);

size_t trudpSendData(trudpData *td, void *data, size_t data_length);
int trudpProcessSendQueue(trudpData *td);
void *trudpProcessReceivedPacket(trudpData *td, void *packet, 
        size_t packet_length, size_t *data_length);
inline void trudpSaveRemoteAddr(trudpData *td, struct sockaddr_in *remaddr, 
        socklen_t addr_length);

#ifdef __cplusplus
}
#endif

#endif /* TR_UDP_H */

