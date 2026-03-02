const std = @import("std");


pub const ForestMQ = struct {
    const Self = @This();

    comptime T: type,

    allocator: std.mem.Allocator,


    pub fn init(comptime T: type) !Self {
       return ForestMQ{
            .T = T,
       };
    }
};
