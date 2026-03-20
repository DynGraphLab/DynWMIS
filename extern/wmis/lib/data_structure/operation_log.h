/******************************************************************************
 * operation_log.h
 *
 * Copyright (C) 2015-2017 Sebastian Lamm <lamm@ira.uka.de>
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
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *****************************************************************************/

#ifndef _OPERATION_LOG_H_
#define _OPERATION_LOG_H_

#include <deque>

#include "definitions.h"

class operation_log {
    public:
        /**
         * Default Constructor.
         */
        operation_log();

        /**
         * Default Destructor.
         */
        virtual ~operation_log();

        /**
         * Get the singleton instance.
         * 
         * @return Instance of the logger.
         */
        static operation_log *instance() {
            static operation_log inst;
            return &inst;
        };
        
        /**
         * Initialize the deque with the given size
         *
         * @param size The size of the deque.
         */
        void init(unsigned int size);
        
        /**
         * Log a insertion of the given node.
         *
         * @param x Node that was inserted.
         */
        void report_insert(NodeID x);

        /**
         * Log a removal of the given node.
         *
         * @param x Node that was removed.
         */
        void report_remove(NodeID x);

        /**
         * Look at a certain position of the deque.
         *
         * @param pos Position to look at.
         * @return Node at the position.
         */
        int peek(unsigned int pos);

        /**
         * Return the latest operation performed.
         *
         * @return Performed operation.
         */
        int unwind();

        /** 
         * Check if the deque is empty.
         *
         * @return True if empty.
         */
        bool is_empty();

        /**
         * Check if the deque is full.
         * 
         * @return True if full.
         */
        bool is_full();

        /**
         * Get the current size of the deque.
         *
         * @return Size of the deque.
         */
        unsigned int get_size();

        /**
         * Log is now active.
         */
        void activate();

        /**
         * Log is now inactive.
         */
        void deactivate();

        /**
         * Reset the log.
         */
        void reset();

        /**
         * Print the current status of the log.
         */
        void print();

    private:
        // Whether or not logging is active.
        bool active;
        // Underlying deque.
        std::deque<int> op_log;
};

#endif

