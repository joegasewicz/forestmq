const std = @import("std");

pub const Message = struct {

    const Self = @This();

    topic: []const u8,
    body: []const u8,
    flags: u8 = 0,

    pub fn init(
        topic: []const u8,
        body: []const u8,
        flags: u8,
    ) Self {
        return Self{
            .topic = topic,
            .body = body,
            .flags = flags,
        };
    }
};

test "Message.init" {
    const m = Message.init("emails", "body message", 0);
    try std.testing.expectEqual("emails", m.topic);
    try std.testing.expectEqual("body message", m.body);
    try std.testing.expectEqual(0, m.flags);
}
