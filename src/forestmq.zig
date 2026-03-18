const std = @import("std");
const topic_module = @import("./topic.zig");

const Topic = topic_module.Topic;

pub const ForestMQ = struct {
    const Self = @This();

    allocator: std.mem.Allocator,
    topics: Topic,

    pub fn init(allocator: std.mem.Allocator) !Self {
       return Self{
            .allocator = allocator,
            .topics = try Topic.init(allocator),
       };
    }

    pub fn deinit(self: *Self) void {
        self.topics.deinit();
        self.* = undefined;
    }

};


test "ForestMQ.init success" {
    var fq = try ForestMQ.init(std.testing.allocator);
    defer fq.deinit();

    try std.testing.expect(fq.topics.topic_map.count() == 0);
}

