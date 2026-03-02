const std = @import("std");
const message = @import("./message.zig");

const Message = message.Message;

pub const Queue = struct {
    const Self = @This();
    allocator: std.mem.Allocator,
    buffer: []*Message,
    head: usize = 0, // Read position
    tail: usize = 0, // Write position
    len: usize = 0,

    pub fn init(allocator: std.mem.Allocator, capacity: usize) !Self {
        if (capacity == 0) return error.InvalidCapacity;

        return Self{
            .allocator = allocator,
            .buffer = try allocator.alloc(*Message, capacity),
        };
    }

    pub fn deinit(self: *Self) void {
        self.allocator.free(self.buffer);
        self.* = undefined;
    }

    pub fn isEmpty(self: *const Self) bool {
        return self.len == 0;
    }

    pub fn isFull(self: *const Self) bool {
       return self.len == self.buffer.len;
    }

    pub fn push(self: *Self, msg: *Message) !void {
        // Modulus will always be the length of the buffer -1 until the
        // buffer is full then = 0.
        if (self.isFull()) return error.QueueFull;

        self.buffer[self.tail] = msg;
        self.tail = (self.tail + 1) % self.buffer.len;
        self.len += 1;
    }

    pub fn pop(self: *Self) !*Message {
        if (self.isEmpty()) return error.QueueEmpty;

        const msg = self.buffer[self.head];
        // Reset the head to the current buffer length.
        // This will be the current length of the buffer or 0.
        self.head = (self.head + 1) % self.buffer.len;
        self.len -= 1;
        return msg;
    }

};
