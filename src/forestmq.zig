const std = @import("std");

pub const ForestMQ = struct {
    const Self = @This();

    allocator: std.mem.Allocator,

    pub fn init(allocator: std.mem.Allocator) !Self {
       return Self{
        .allocator = allocator,
       };
    }
};


test "ForestMQ.init success" {
    const fq = try ForestMQ.init(std.testing.allocator);
    _ = fq;
}

