
pub const Message = struct {

    const Self = @This();

    topic: []const u8,
    body: []const u8,
    flags: u8 = 0,

    pub fn init(
        topic: []const u8,
        body: []const u8,
        flags: []const u8,
    ) Self {
        return Self{
            .topic = topic,
            .body = body,
            .flags = flags,
        };
    }
};
