
#[derive(Debug)]
pub enum ZsignError {
    Msg(String),
}

unsafe impl Send for ZsignError {}
unsafe impl Sync for ZsignError {}

