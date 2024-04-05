use std::fmt::{Display, Formatter};

#[derive(Debug)]
pub enum ZsignError {
    Msg(String),
}

impl Display for ZsignError {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        match self {
            ZsignError::Msg(msg) => write!(f, "{}", msg),
        }
    }
}

impl std::error::Error for ZsignError {

}

unsafe impl Send for ZsignError {}

unsafe impl Sync for ZsignError {}

