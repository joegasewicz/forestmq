const std = @import("std");
const message = @import("./message.zig");

const Message = message.Message;

pub const Queue = struct {
    const Self = @This();
    allocator: std.mem.Allocator,
    buffer: []*Message,
    head: usize = 0,
    tail: usize = 0,
    len: usize = 0,

    pub fn init(allocator: std.mem.Allocator, capacity: usize) !Self {
        if (capacity == 0) return error.InvalidCapacity;

        return Self{
            .allocator = allocator,
            .buffer = try allocator.alloc(*Message, capacity),
        };
    }


};
