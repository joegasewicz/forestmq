const std = @import("std");


pub const ForestMQ = struct {
    const Self = @This();

    allocator: std.mem.Allocator,


    pub fn init() !Self {
       return ForestMQ{

       };
    }
};
