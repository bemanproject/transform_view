import React from 'react';

export default function Paper({num}) {
    return (
        <a href={"https://wg21.link/" + num}>{num}</a>
    );
}
