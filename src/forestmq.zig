const std = @import("std");
const config_mod = @import("./config.zig");
const message_mod = @import("./message.zig");
const topic_mod = @import("./topic.zig");

const log = std.log;
const Message = message_mod.Message;
const Topic = topic_mod.Topic;

pub const ForestMQ = struct {
    const Self = @This();

    allocator: std.mem.Allocator,
    topics: Topic,

    pub fn init(allocator: std.mem.Allocator) !Self {
        var buf: [64]u8 = undefined;
        try config_mod.printPreamble(&buf);
        return Self{
            .allocator = allocator,
            .topics = try Topic.init(allocator),
        };
    }

    pub fn deinit(self: *Self) void {
        log.debug("Deinitizing ForestMQ...", .{});
        self.topics.deinit();
        self.* = undefined;
    }

    pub fn createTopic(self: *Self, name: []const u8, capacity: usize) !void {
        log.info("Creating new topic: {s}" , .{name});
        try self.topics.add(name, capacity);
    }

    pub fn deleteTopic(self: *Self, name: []const u8) !void {
        try self.topics.remove(name);
    }

    pub fn publish(self: *Self, topic_name: []const u8, msg: *Message) !void {
        try self.topics.push(topic_name, msg);
    }

    pub fn consume(self: *Self, topic_name: []const u8) !*Message {
        return self.topics.pop(topic_name);
    }
};


test "ForestMQ.init success" {
    var fq = try ForestMQ.init(std.testing.allocator);
    defer fq.deinit();

    try std.testing.expect(fq.topics.topic_map.count() == 0);
}

test "ForestMQ.deinit success" {
    var fq = try ForestMQ.init(std.testing.allocator);
    defer fq.deinit();
}

test "ForestMQ.createTopic" {
    var fq = try ForestMQ.init(std.testing.allocator);
    defer fq.deinit();

    try fq.createTopic("emails", 2);
    const key = fq.topics.topic_map.getKey("emails");
    try std.testing.expectEqualStrings("emails", key.?);
}

test "ForestMQ.deleteTopic" {
    var fq = try ForestMQ.init(std.testing.allocator);
    defer fq.deinit();

    try fq.createTopic("emails", 2);
    try std.testing.expect(fq.topics.get("emails") != null);
    try std.testing.expect(fq.topics.get("emails2") == null);
}

test "ForestMQ.publish" {
    var fq = try ForestMQ.init(std.testing.allocator);
    defer fq.deinit();

    try fq.createTopic("emails", 2);
    try std.testing.expect(fq.topics.get("emails") != null);

    var msg1 = Message.init("emails", "message #1", 0);
    try fq.publish("emails", &msg1);
}

test "ForestMQ.pop" {
    var fq = try ForestMQ.init(std.testing.allocator);
    defer fq.deinit();

    try fq.createTopic("emails", 2);
    try std.testing.expect(fq.topics.get("emails") != null);

    var msg1 = Message.init("emails", "message #1", 0);
    try fq.publish("emails", &msg1);

    const popped_msg1 = try fq.consume("emails");
    try std.testing.expectEqual("emails", popped_msg1.topic);
}