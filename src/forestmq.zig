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
       config_mod.printPreamble();
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
};


test "ForestMQ.init success" {
    var fq = try ForestMQ.init(std.testing.allocator);
    defer fq.deinit();

    try std.testing.expect(fq.topics.topic_map.count() == 0);
}

test "ForestMQ.createTopic" {
    var fq = try ForestMQ.init(std.testing.allocator);
    defer fq.deinit();

    try fq.createTopic("emails", 2);
    const key = fq.topics.topic_map.getKey("emails");
    try std.testing.expectEqualStrings("emails", key.?);
}