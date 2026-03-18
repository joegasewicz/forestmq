const std = @import("std");
const message = @import("./message.zig");

const log = std.log;
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
        log.info("Dequeued message successfully", .{});
        return msg;
    }

    pub fn length(self: *Self) usize {
        return self.len;
    }

};

test "Queue.init" {
    var q = try Queue.init(std.testing.allocator, 10);
    defer q.deinit();
    try std.testing.expectEqual(0, q.head);
    try std.testing.expectEqual(0, q.tail);
    try std.testing.expectEqual(0, q.len);
}

test "Queue.deinit" {
    var q = try Queue.init(std.testing.allocator, 10);
    q.deinit();
}

test "Queue.isEmpty" {
    var q = try Queue.init(std.testing.allocator, 10);
    defer q.deinit();
    try std.testing.expect(q.isEmpty());

    // Test isEmpty returns false
    var msg = Message{
        .topic = "test",
        .body = "hello"
    };
    try q.push(&msg);
    try std.testing.expect(q.isEmpty() == false);
}

test "Queue.isFull" {
    var q = try Queue.init(std.testing.allocator, 1);
    defer q.deinit();
    try std.testing.expect(q.isFull() == false);

    // test queue is full
    var msg = Message{
        .topic = "test",
        .body = "hello"
    };
    try q.push(&msg);
    try std.testing.expect(q.isFull());
}

test "Queue.push" {
    var q = try Queue.init(std.testing.allocator, 3);
    defer q.deinit();
    var msg1 = Message{
        .topic = "test1",
        .body = "hello1"
    };
    var msg2 = Message{
        .topic = "test2",
        .body = "hello2"
    };
    try q.push(&msg1);
    try q.push(&msg2);

    try std.testing.expectEqual("test1", q.buffer[0].topic);
    try std.testing.expectEqual("hello1", q.buffer[0].body);
    try std.testing.expectEqual("test2", q.buffer[1].topic);
    try std.testing.expectEqual("hello2", q.buffer[1].body);
}

test "Queue.pop" {
    var q = try Queue.init(std.testing.allocator, 3);
    defer q.deinit();
    var msg1 = Message{
        .topic = "test1",
        .body = "hello1"
    };
    var msg2 = Message{
        .topic = "test2",
        .body = "hello2"
    };
    try q.push(&msg1);
    try q.push(&msg2);

    const message1 = try q.pop();
    const message2 = try q.pop();

    try std.testing.expectEqual("test1", message1.topic);
    try std.testing.expectEqual("hello1", message1.body);
    try std.testing.expectEqual("test2", message2.topic);
    try std.testing.expectEqual("hello2", message2.body);
}

test "Queue.length" {
    var q = try Queue.init(std.testing.allocator, 3);
    defer q.deinit();
    var msg1 = Message{
        .topic = "test1",
        .body = "hello1"
    };
    var msg2 = Message{
        .topic = "test2",
        .body = "hello2"
    };

    try std.testing.expectEqual(@as(usize, 0), q.length());
    try q.push(&msg1);
    try std.testing.expectEqual(@as(usize, 1), q.length());
    try q.push(&msg2);
    try std.testing.expectEqual(@as(usize, 2), q.length());
}