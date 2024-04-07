use std::fmt::{Display, Formatter};

#[derive(Debug)]
pub enum ZsignError {
    Msg(String),
    StdError(Box<dyn std::error::Error>),
}

impl Display for ZsignError {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        match self {
            ZsignError::Msg(msg) => write!(f, "{}", msg),
            ZsignError::StdError(err) => write!(f, "{}", err),
        }
    }
}
impl ZsignError{
    pub fn msg(msg: impl AsRef<str>) -> Self {
        ZsignError::Msg(msg.as_ref().to_string())
    }
    pub fn from(err: impl std::error::Error + 'static) -> Self {
        ZsignError::StdError(Box::new(err))
    }

}

impl std::error::Error for ZsignError {}

unsafe impl Send for ZsignError {}

unsafe impl Sync for ZsignError {}
