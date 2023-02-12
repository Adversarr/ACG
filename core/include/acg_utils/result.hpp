#pragma once
#include <utility>

#include "log.hpp"

#define ACG_CHECK_STATUS_OK(r, ...) \
  ACG_CHECK((r) == acg::Status::kOk, "Status expr[" #r "] check failed." __VA_ARGS__)

namespace acg {
// Reference: Google/Abseil
enum class Status {
  // StatusCode::kOk
  //
  // kOK (gRPC code "OK") does not indicate an error; this value is returned on
  // success. It is typical to check for this value before proceeding on any
  // given call across an API or RPC boundary. To check this value, use the
  // `absl::Status::ok()` member function rather than inspecting the raw code.
  kOk = 0,

  // StatusCode::kCancelled
  //
  // kCancelled (gRPC code "CANCELLED") indicates the operation was cancelled,
  // typically by the caller.
  kCancelled = 1,

  // StatusCode::kUnknown
  //
  // kUnknown (gRPC code "UNKNOWN") indicates an unknown error occurred. In
  // general, more specific errors should be raised, if possible. Errors raised
  // by APIs that do not return enough error information may be converted to
  // this error.
  kUnknown = 2,

  // StatusCode::kInvalidArgument
  //
  // kInvalidArgument (gRPC code "INVALID_ARGUMENT") indicates the caller
  // specified an invalid argument, such as a malformed filename. Note that use
  // of such errors should be narrowly limited to indicate the invalid nature of
  // the arguments themselves. Errors with validly formed arguments that may
  // cause errors with the state of the receiving system should be denoted with
  // `kFailedPrecondition` instead.
  kInvalidArgument = 3,

  // StatusCode::kDeadlineExceeded
  //
  // kDeadlineExceeded (gRPC code "DEADLINE_EXCEEDED") indicates a deadline
  // expired before the operation could complete. For operations that may change
  // state within a system, this error may be returned even if the operation has
  // completed successfully. For example, a successful response from a server
  // could have been delayed long enough for the deadline to expire.
  kDeadlineExceeded = 4,

  // StatusCode::kNotFound
  //
  // kNotFound (gRPC code "NOT_FOUND") indicates some requested entity (such as
  // a file or directory) was not found.
  //
  // `kNotFound` is useful if a request should be denied for an entire class of
  // users, such as during a gradual feature rollout or undocumented allow list.
  // If a request should be denied for specific sets of users, such as through
  // user-based access control, use `kPermissionDenied` instead.
  kNotFound = 5,

  // StatusCode::kAlreadyExists
  //
  // kAlreadyExists (gRPC code "ALREADY_EXISTS") indicates that the entity a
  // caller attempted to create (such as a file or directory) is already
  // present.
  kAlreadyExists = 6,

  // StatusCode::kPermissionDenied
  //
  // kPermissionDenied (gRPC code "PERMISSION_DENIED") indicates that the caller
  // does not have permission to execute the specified operation. Note that this
  // error is different than an error due to an *un*authenticated user. This
  // error code does not imply the request is valid or the requested entity
  // exists or satisfies any other pre-conditions.
  //
  // `kPermissionDenied` must not be used for rejections caused by exhausting
  // some resource. Instead, use `kResourceExhausted` for those errors.
  // `kPermissionDenied` must not be used if the caller cannot be identified.
  // Instead, use `kUnauthenticated` for those errors.
  kPermissionDenied = 7,

  // StatusCode::kResourceExhausted
  //
  // kResourceExhausted (gRPC code "RESOURCE_EXHAUSTED") indicates some resource
  // has been exhausted, perhaps a per-user quota, or perhaps the entire file
  // system is out of space.
  kResourceExhausted = 8,

  // StatusCode::kFailedPrecondition
  //
  // kFailedPrecondition (gRPC code "FAILED_PRECONDITION") indicates that the
  // operation was rejected because the system is not in a state required for
  // the operation's execution. For example, a directory to be deleted may be
  // non-empty, an "rmdir" operation is applied to a non-directory, etc.
  //
  // Some guidelines that may help a service implementer in deciding between
  // `kFailedPrecondition`, `kAborted`, and `kUnavailable`:
  //
  //  (a) Use `kUnavailable` if the client can retry just the failing call.
  //  (b) Use `kAborted` if the client should retry at a higher transaction
  //      level (such as when a client-specified test-and-set fails, indicating
  //      the client should restart a read-modify-write sequence).
  //  (c) Use `kFailedPrecondition` if the client should not retry until
  //      the system state has been explicitly fixed. For example, if a "rmdir"
  //      fails because the directory is non-empty, `kFailedPrecondition`
  //      should be returned since the client should not retry unless
  //      the files are deleted from the directory.
  kFailedPrecondition = 9,

  // StatusCode::kAborted
  //
  // kAborted (gRPC code "ABORTED") indicates the operation was aborted,
  // typically due to a concurrency issue such as a sequencer check failure or a
  // failed transaction.
  //
  // See the guidelines above for deciding between `kFailedPrecondition`,
  // `kAborted`, and `kUnavailable`.
  kAborted = 10,

  // StatusCode::kOutOfRange
  //
  // kOutOfRange (gRPC code "OUT_OF_RANGE") indicates the operation was
  // attempted past the valid range, such as seeking or reading past an
  // end-of-file.
  //
  // Unlike `kInvalidArgument`, this error indicates a problem that may
  // be fixed if the system state changes. For example, a 32-bit file
  // system will generate `kInvalidArgument` if asked to read at an
  // offset that is not in the range [0,2^32-1], but it will generate
  // `kOutOfRange` if asked to read from an offset past the current
  // file size.
  //
  // There is a fair bit of overlap between `kFailedPrecondition` and
  // `kOutOfRange`.  We recommend using `kOutOfRange` (the more specific
  // error) when it applies so that callers who are iterating through
  // a space can easily look for an `kOutOfRange` error to detect when
  // they are done.
  kOutOfRange = 11,

  // StatusCode::kUnimplemented
  //
  // kUnimplemented (gRPC code "UNIMPLEMENTED") indicates the operation is not
  // implemented or supported in this service. In this case, the operation
  // should not be re-attempted.
  kUnimplemented = 12,

  // StatusCode::kInternal
  //
  // kInternal (gRPC code "INTERNAL") indicates an internal error has occurred
  // and some invariants expected by the underlying system have not been
  // satisfied. This error code is reserved for serious errors.
  kInternal = 13,

  // StatusCode::kUnavailable
  //
  // kUnavailable (gRPC code "UNAVAILABLE") indicates the service is currently
  // unavailable and that this is most likely a transient condition. An error
  // such as this can be corrected by retrying with a backoff scheme. Note that
  // it is not always safe to retry non-idempotent operations.
  //
  // See the guidelines above for deciding between `kFailedPrecondition`,
  // `kAborted`, and `kUnavailable`.
  kUnavailable = 14,

  // StatusCode::kDataLoss
  //
  // kDataLoss (gRPC code "DATA_LOSS") indicates that unrecoverable data loss or
  // corruption has occurred. As this error is serious, proper alerting should
  // be attached to errors such as this.
  kDataLoss = 15,

  // StatusCode::kUnauthenticated
  //
  // kUnauthenticated (gRPC code "UNAUTHENTICATED") indicates that the request
  // does not have valid authentication credentials for the operation. Correct
  // the authentication and try again.
  kUnauthenticated = 16,

  // StatusCode::DoNotUseReservedForFutureExpansionUseDefaultInSwitchInstead_
  //
  // NOTE: this error code entry should not be used and you should not rely on
  // its value, which may change.
  //
  // The purpose of this enumerated value is to force people who handle status
  // codes with `switch()` statements to *not* simply enumerate all possible
  // values, but instead provide a "default:" case. Providing such a default
  // case ensures that code will compile when new codes are added.
  kDoNotUseReservedForFutureExpansionUseDefaultInSwitchInstead = 20
};


template <typename T> class Result;

template <> class Result<void> {
public:
  explicit Result(Status err = Status::kOk) : is_inited_(err == Status::kOk), error_(err) {}

  Result(Status err, std::string additional_message)
      : is_inited_(false), error_(err), additional_message_(std::move(additional_message)) {}

  static void Value() { ACG_CHECK(false, "Bad Access Error"); }

  Status Error() const {
    ACG_CHECK(!HasValue(), "Bad access error");
    return error_;
  }

  bool HasValue() const { return is_inited_; }

  explicit operator bool() const { return HasValue(); }

  void operator*() const {}

  const std::string& GetMessage() const {
    ACG_CHECK(!HasValue(), "Bad Access Error");
    return additional_message_;
  }

  bool is_inited_ = false;
  Status error_ = Status::kOk;
  std::string additional_message_;
};

template <typename T> class Result final {
  static_assert(!std::is_reference_v<T>, "Result cannot contains a reference.");

public:
  using E = Status;
  template <typename... Args> explicit Result(Args&&... args)
      : value_(std::forward<Args...>(args)...), is_inited_(true) {}

  explicit Result(T&& data) : value_(std::move(data)), is_inited_(true) {}

  explicit Result(const T& data) : value_(data), is_inited_(true) {}

  explicit Result(E err) : error_(err), is_inited_(false) {}

  explicit Result(E err, std::string additional_message)
      : error_(err), is_inited_(false), additional_message_(std::move(additional_message)) {}

  // NOLINTBEGIN(google-explicit-constructor)
  Result(const Result<void>& rhs)
      : error_(rhs.Error()),
        additional_message_(rhs.additional_message_),
        is_inited_(rhs.HasValue()) {
    ACG_DEBUG_CHECK(!HasValue(), "Cannot convert from Result<void> when it has no error.");
  }
  // NOLINTEND(google-explicit-constructor)

  Result& operator=(const Result<void>& rhs) {
    is_inited_ = rhs.is_inited_;
    ACG_DEBUG_CHECK(!HasValue(), "Cannot convert from Result<void> when it has no error.");
    additional_message_ = rhs.additional_message_;
  }

  // Disable Copy
  Result(const Result&) = delete;

  // Enable move
  Result(Result&& another) : is_inited_(another.is_inited_) {
    if (is_inited_)
      new (&value_) T(std::move(another.value_));
    else
      new (&error_) E(std::move(another.error_));
  }

  template <typename... Args> static Result<T> Emplace(Args&&... data) {
    return Result(T(data...));
  }

  const T& Value() const {
    ACG_CHECK(HasValue(), "Bad Access Error");
    return value_;
  }

  T& Value() {
    ACG_CHECK(HasValue(), "Bad Access Error");
    return value_;
  }

  E Error() const {
    ACG_CHECK(!HasValue(), "Bad Access Error");
    return error_;
  }

  const std::string& GetMessage() const {
    ACG_CHECK(!HasValue(), "Bad Access Error");
    return additional_message_;
  }

  bool HasValue() const { return is_inited_; }

  explicit operator bool() const { return HasValue(); }

  T& operator*() { return Value(); }

  const T& operator*() const { return Value(); }

private:
  union {
    T value_;
    E error_;
  };

  std::string additional_message_;
  bool is_inited_;
};

template <typename T, typename... Args> auto make_result(Args&&... args) {
  return Result<T>(std::forward<Args...>(args)...);
}

template <typename T = void> auto make_error(Status err) { return Result<T>(err); }
template <typename T = void> auto make_error(Status err, std::string additional_message) {
  return Result<T>(err, std::move(additional_message));
}

}  // namespace acg
