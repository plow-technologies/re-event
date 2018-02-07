# re-event
Simple event messaging.

This is useful sometimes to communicate between components in ReasonReact.

### How it works?
It piggybacks on existing browser's `Event`. It monkey patches the `Event` object to add additional property called `payload`. That's it.

Why not use `CustomEvent`? Because older IE doesn't support the constructor and based on MDN, some of the methods to construct custom event are deprecated. I am not entirely aware of any trade off of monkey patching the `Event` object itself.

# Usage

```reason
type payload = {...}; /* it doesn't have to be this particular type. This is an example. */

let encodePayload = (payload: payload) : Js.Json.t => {...};
let decodePayload = (json: Js.Json.t) : payload => {...};

let make = (_children) => {
  ...component,
  initialState,
  reducer,
  subscriptions: (self) => [
    Sub(
      () =>
        REvent.on(
          "notify",
          (event) =>
            switch (REvent.getPayload(event, decodePayload)) {
            | None => ()
            | Some(payload) => self.send(UpdateSomeState(payload))
            }
        ),
      (listener) => REvent.off("notify", listener) /* or curried version, REvent.off("notify") */
    )
  ],
  render: (self) => <div />
};

/* Fire an event */
let payload = {...};
let ev = REvent.createEvent("notify", Some(encodePayload(payload))); /* None for no payload */
REvent.emit(ev);
```

The payload you sent in a way is typed. Although the type is `Js.Json.t`, it can only be decoded with the correct decoder. It will return `None` if decoding failed.
