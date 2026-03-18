const std = @import("std");
const queue_mod = @import("./queue.zig");

const Queue = queue_mod.Queue;

pub const Topic = struct {
    const Self = @This();

    allocator: std.mem.Allocator,
    topics: std.StringHashMap(Queue),

    pub fn init(allocator: std.mem.Allocator) !Self {
        return Self{
            .allocator = allocator,
            .topics = std.StringArrayHashMap(Queue).init(allocator),
        };
    }

    pub fn deinit(self: *Self) void {
        var iterator = self.topics.iterator();
        while (iterator.next()) |entry| {
            self.allocator.free(entry.key_ptr.*);
            entry.value_ptr.deinit();
        }
        self.* = undefined;
    }

    pub fn add(self: *Self, name: []const u8, capacity: usize) !void {
        if (self.topics.cotains(name)) {
            return error.TopicAlreadyExists;
        }

        const owned_name = try self.allocator.dupe(u8, name);
        errdefer self.allocator.free(owned_name);

        const queue = try Queue.init(self.allocator, capacity);
        errdefer {
            var q = queue;
            q.deinit();
        }

        try self.topics.put(owned_name, queue);
    }
};
