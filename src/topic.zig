const std = @import("std");
const queue_mod = @import("./queue.zig");
const message_mod = @import("./message.zig");

const log = std.log;
const Message = message_mod.Message;
const Queue = queue_mod.Queue;

pub const Topic = struct {
    const Self = @This();

    allocator: std.mem.Allocator,
    topic_map: std.StringHashMap(Queue),

    pub fn init(allocator: std.mem.Allocator) !Self {
        return Self{
            .allocator = allocator,
            .topic_map = std.StringHashMap(Queue).init(allocator),
        };
    }

    pub fn deinit(self: *Self) void {
        var iterator = self.topic_map.iterator();
        while (iterator.next()) |entry| {
            const key = entry.key_ptr.*;
            self.allocator.free(key);
            log.debug("Deinitializing {s} queue...", .{key});
            entry.value_ptr.deinit(); // Deinit each queue
        }
        self.topic_map.deinit();
        self.* = undefined;
    }

    pub fn add(self: *Self, name: []const u8, capacity: usize) !void {
        if (self.topic_map.contains(name)) {
            return error.TopicAlreadyExists;
        }

        const owned_name = try self.allocator.dupe(u8, name);
        errdefer self.allocator.free(owned_name);

        const queue = try Queue.init(self.allocator, capacity);
        errdefer {
            var q = queue;
            q.deinit();
        }

        try self.topic_map.put(owned_name, queue);
    }

    pub fn get(self: *Self, name: []const u8) ?*Queue {
       return self.topic_map.getPtr(name);
    }

    pub fn remove(self: *Self, name: []const u8) !void {
        const removed = self.topic_map.fetchRemove(name) orelse return error.TopicNotFound;

        self.allocator.free(removed.key);
        var queue = removed.value;
        queue.deinit();
    }

    pub fn push(self: *Self, topic: []const u8, msg: *Message) !void {
        const q = self.get(topic) orelse return error.TopicNotFound;
        try q.push(msg);
    }

    pub fn pop(self: *Self, topic: []const u8) !*Message {
        const q = self.get(topic) orelse return error.TopicNotFound;
        return q.pop();
    }
};

test "Topic.init" {
    var t = try Topic.init(std.testing.allocator);
    defer t.deinit();
    try std.testing.expectEqual(@as(usize, 0), t.topic_map.count());
}

test "Topic.deinit" {
    var t = try Topic.init(std.testing.allocator);
    t.deinit();
}

test "Topic.add" {
    var t = try Topic.init(std.testing.allocator);
    defer t.deinit();

    try t.add("emails", 2);

    const q = t.get("emails");

    try std.testing.expect(q != null);
    try std.testing.expect(q.?.isEmpty());
    try std.testing.expectEqual(@as(usize, 2), q.?.buffer.len);
}

test "Topic.remove" {
    var t = try Topic.init(std.testing.allocator);
    defer t.deinit();

    try t.add("emails", 1);
    try t.add("agents", 1);

    try t.remove("emails");
    const emailQueue = t.get("emails");
    const agentQueue = t.get("agents");

    try std.testing.expect(emailQueue == null);
    try std.testing.expect(agentQueue != null);
}

test "Topic.get" {
    var t = try Topic.init(std.testing.allocator);
    defer t.deinit();

    try t.add("emails", 1);
    try t.add("agents", 1);

    const emailQueue = t.get("emails");
    try std.testing.expect(emailQueue != null);
}

test "Topic.push" {
    var t = try Topic.init(std.testing.allocator);
    defer t.deinit();

    try t.add("emails", 1);
    try t.add("agents", 1);

    const emailQueue = t.get("emails");
    var msg1 = Message.init("emails", "message #1", 0);
    try emailQueue.?.push(&msg1);

    const msgPopped = try emailQueue.?.pop();
    try std.testing.expectEqual("emails", msgPopped.topic);
}

test "Topic.pop" {
    var t = try Topic.init(std.testing.allocator);
    defer t.deinit();

    try t.add("emails", 3);
    try t.add("agents", 1);

    const emailQueue = t.get("emails");
    var msg1 = Message.init("emails", "message #1", 0);
    var msg2 = Message.init("emails", "message #2", 0);
    var msg3 = Message.init("emails", "message #3", 0);
    try emailQueue.?.push(&msg1);
    try emailQueue.?.push(&msg2);
    try emailQueue.?.push(&msg3);

    const msgPopped1 = try emailQueue.?.pop();
    const msgPopped2 = try emailQueue.?.pop();
    const msgPopped3 = try emailQueue.?.pop();
    try std.testing.expectEqual("message #1", msgPopped1.body);
    try std.testing.expectEqual("message #2", msgPopped2.body);
    try std.testing.expectEqual("message #3", msgPopped3.body);
}