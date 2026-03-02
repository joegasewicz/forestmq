pub const Message = struct {
    topic: []const u8,
    body: []u8,
    flags: u8 = 0,
};
